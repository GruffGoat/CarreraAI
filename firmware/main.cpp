#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <hwlib.h>
#include <socal/socal.h>
#include <socal/hps.h>
#include <sys/ioctl.h>
#include <signal.h>
#include "hps_0.h"
#include "main.h"
#include "DataAcquisition.h"
#include "MatlabConnect/MatlabTCP.h"
#include "Controller.h"
#include "Correlation.h"
#include "lsm9d1.h"

const bool DoCorrelation = true;
const bool DoLookAheadSpeedControlling = true;

const double MAX_DECELERATION = 450; // in mm/s² //450
const double MAX_TURN_ACCELERATION = 6150; // in mm/s² //5950

const int HISTORY_LENGTH = 300; // How much history to store for cross-correlation
const int SHIFT = 35; // how far to shift for correlation

typedef struct timespec timespec;
timespec TimeDifference(timespec start, timespec end);

void* VirtualBaseAddress = 0;
int MemoryFileDescriptor = 0;
volatile unsigned long* MotorControlAddress = 0;
volatile unsigned long* CarSensorsAddress = 0;
volatile unsigned long* CarLedsAddress = 0;
volatile unsigned long* LedsAddress = 0;
volatile bool Stop = false;
volatile std::atomic<double> MaxSpeed(3800);

void SignalHandler(int signalNumber)
{
	std::cerr << "Received signal" << std::endl;

	std::cerr << "Stopping engine" << std::endl;
	Controller::SetSetpoint(0);
	if(MotorControlAddress) {
		while(alt_read_word(MotorControlAddress+2) != 0) {
			alt_write_word(MotorControlAddress+2, 0);
		}

		uint32_t controlRegister = alt_read_word(MotorControlAddress);
		controlRegister &= ~(0x2); // inhibit on
		alt_write_word(MotorControlAddress, controlRegister);
	}

	std::cerr << "Stopping matlab tcp" << std::endl;
	if(MatlabTCP::GetInstance()) {
		MatlabTCP::GetInstance()->Stop();
	}

	std::cerr << "Stopping data acquisition" << std::endl;
	if(DataAcquisition::GetInstance()) {
		DataAcquisition::GetInstance()->Stop();
	}

	// Stop engine again
	if(MotorControlAddress) {
		alt_write_word(MotorControlAddress+2, 0);
		uint32_t controlRegister = alt_read_word(MotorControlAddress);
		controlRegister &= ~(0x2); // inhibit on
		alt_write_word(MotorControlAddress, controlRegister);
	}

	std::cerr << "Unmapping memory" << std::endl;
	if(munmap(VirtualBaseAddress, HW_REGS_SPAN ) != 0) {
		fprintf(stderr, "ERROR: munmap() failed...\n");
		close(MemoryFileDescriptor);
	}
	close(MemoryFileDescriptor);

	//stop
	exit(0);
}

int main(int argc, char **argv)
{
	if (signal(SIGINT, SignalHandler) == SIG_ERR) {
		fprintf(stderr, "Failed to attach SIGINT handler\n");
	}
	if (signal(SIGTERM, SignalHandler) == SIG_ERR) {
		fprintf(stderr, "Failed to attach SIGINT handler\n");
	}
	signal(SIGPIPE, SIG_IGN);

	// Get reference speed from command line parameter, use initialisation value if none is provided.
	double speedSetting = 1000.0;
	if(argc > 1) {
		speedSetting = atof(argv[1]);
	}
	Controller::SetSetpoint(speedSetting);

	// Map the address space of the FPGA registers into user space so we can interact with them.
	errno = 0;
	MemoryFileDescriptor = open("/dev/mem", (O_RDWR | O_SYNC));
	if(MemoryFileDescriptor == -1) {
		perror("Error opening /dev/mem");
		return 1;
	}

	errno = 0;
	VirtualBaseAddress = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, MemoryFileDescriptor, HW_REGS_BASE);
	if(VirtualBaseAddress == MAP_FAILED) {
		perror("Error executing mmap");
		close(MemoryFileDescriptor);
		return 1;
	}

	// Calculate addresses
	MotorControlAddress = (unsigned long *)((unsigned long)VirtualBaseAddress + ((unsigned long)(ALT_LWFPGASLVS_OFST + MOTOR_CONTROL_0_BASE) & (unsigned long)(HW_REGS_MASK)));
	CarSensorsAddress = (unsigned long *)((unsigned long)VirtualBaseAddress + ((unsigned long)(ALT_LWFPGASLVS_OFST + CARSENSORS_BASE) & (unsigned long)(HW_REGS_MASK)));
	CarLedsAddress = (unsigned long *)((unsigned long)VirtualBaseAddress + ((unsigned long)(ALT_LWFPGASLVS_OFST + CARLEDS_BASE) & (unsigned long)(HW_REGS_MASK)));
	LedsAddress =  (unsigned long *)((unsigned long)VirtualBaseAddress + ((unsigned long)(ALT_LWFPGASLVS_OFST + PIO_LED_BASE) & (unsigned long)(HW_REGS_MASK)));

	// If system freezes after this output there is a problem with the FPGA (registers don’t exist)
	std::cerr << "Accessing FPGA" << std::endl;
	alt_write_word(CarLedsAddress, 1 << CAR_LED_HEADLIGHT_SHIFT); // turn on front leds
	alt_write_word(MotorControlAddress, 0x03); // inhibit off and enable PWM
	std::cerr << "FPGA successfully accessed" << std::endl;

	// Print data output header
	printf("Time (s); Distance (mm); Speed (mm/s); Z-Angle (mdps); Distance corrected (mm); Target Speed; PWM Value\n");

	std::vector<distanceYawRate_t> track;
	std::vector<double> angularRateHistory;
	angularRateHistory.reserve(HISTORY_LENGTH);
	double distanceError = 0;
	double lapStartDistance = 0;
	double lapLength = 0.0;
	uint32_t previousLapCount = 0;

	// Start threads
	MatlabTCP::GetInstance()->Start();
	DataAcquisition::GetInstance()->Start();

	while(true) {
		// Get data sample from data acquisition thread
		dataSample_t dataSample;
		DataAcquisition::GetInstance()->GetData(dataSample);

		if(Stop && dataSample.lapCount > 1) {
			alt_write_word(CarLedsAddress, 0); // turn off LEDs
			alt_write_word(MotorControlAddress, 0x01); // inhibit on (brake) and enable PWM
			alt_write_word(MotorControlAddress+2, 700); // PWM reg
			continue;
		}

		// Detect lap start
		if(previousLapCount != dataSample.lapCount) {
			// Just finished the first (recording) lap
			if(dataSample.lapCount == 2) {
				lapLength = dataSample.distance - lapStartDistance;
			}
			std::cout << "Lap length: " << dataSample.distance - lapStartDistance << std::endl;
			lapStartDistance = dataSample.distance;
			distanceError = 0.0;
			previousLapCount = dataSample.lapCount;
		}

		// Record first Lap (i.e. after we cross the start line for the first time)
		if(dataSample.lapCount == 1) {
			distanceYawRate_t datapoint;
			datapoint.distance = dataSample.distance - lapStartDistance;
			datapoint.yawRate = dataSample.yawRate;
			track.push_back(datapoint);
		}

		if(dataSample.lapCount > 2 && ((dataSample.distance - lapStartDistance) > (lapLength * 1.1))) {
			std::cerr << "Looks like we failed to detect a lap, aborting …" << std::endl;
			SignalHandler(SIGTERM);
		}

		double distanceCorrected = dataSample.distance - distanceError - lapStartDistance;
		std::vector<distanceYawRate_t>::iterator trackPosition;
		// Correct position using cross-correlation
		if(DoCorrelation) {
			// Add to history
			if(angularRateHistory.size() >= HISTORY_LENGTH) {
				// remove oldest entry
				angularRateHistory.erase(angularRateHistory.begin());
			}
			angularRateHistory.push_back(dataSample.yawRate);

			if(dataSample.lapCount > 1) {
				timespec currentTime, timeBefore;
				clock_gettime(CLOCK_MONOTONIC, &timeBefore);

				trackPosition = GetPatternPosition(track, angularRateHistory, dataSample.distance-lapStartDistance, SHIFT); //TODO: Use distanceCorrected?

				clock_gettime(CLOCK_MONOTONIC, &currentTime);
				timespec timeForCalculationTimeSpec = TimeDifference(timeBefore, currentTime);
				double timeForCalculation = timeForCalculationTimeSpec.tv_sec*1000 + (double)timeForCalculationTimeSpec.tv_nsec/MILLION;

				//printf("Match at position (calculated in %f ms): '%f', corrected: '%f', uncorrected: '%f'\n", timeForCalculation, trackPosition->distance, distanceCorrected, dataSample.distance - lapStartDistance);

				// Only start correcting after the first 1000 mm
				if(dataSample.distance - lapStartDistance > 1000) {
					distanceError = dataSample.distance - trackPosition->distance - lapStartDistance;
				}
			}
		} else if (dataSample.lapCount > 1) {
			trackPosition = track.begin();
			while((trackPosition->distance < (dataSample.distance - lapStartDistance)) && (trackPosition != track.end())) {
				trackPosition++;
			}
		}

		if(DoLookAheadSpeedControlling && dataSample.lapCount > 1) {
			speedSetting = MaxSpeed;
			bool wrapAround = false;
			// Look at next 50 samples and determine if we should brake
			for(size_t i = 0; i < 70; i++) {
				// Avoid division by 0
				if(trackPosition->yawRate == 0.0) {
					trackPosition->yawRate = 0.0001;
				}

				// Maximum possible speed for recorded point
				double vMax = sqrt(MAX_TURN_ACCELERATION*90E3/abs(trackPosition->yawRate));
				double brakingDistance = ((dataSample.speed - vMax)*(dataSample.speed - vMax))/(2*MAX_DECELERATION);

				// No need to brake if current speed is already slower
				if(vMax > dataSample.speed) {
					brakingDistance = 0;
				}

				// Brake to vMax if we are within braking distance
				if(wrapAround) {
					// wrap around -> trackPosition is in next lap, add lapLength
					if(brakingDistance > ((trackPosition->distance + lapLength) - distanceCorrected)) {
						if(vMax < speedSetting) {
							speedSetting = vMax;
						}
					}
				} else {
					if(brakingDistance > (trackPosition->distance - distanceCorrected)) {
						if(vMax < speedSetting) {
							speedSetting = vMax;
						}
					}
				}


				trackPosition++;
				if(trackPosition == track.end()) {
					wrapAround = true;
					trackPosition = track.begin();
				}
			}

			// Limit with vMax based on current speed and yawRate
			double vMaxCurrentYawRate = sqrt(MAX_TURN_ACCELERATION*90E3/abs(dataSample.yawRate));
			if(vMaxCurrentYawRate < speedSetting) {
				speedSetting = vMaxCurrentYawRate;
			}

			// Turn off headlight LEDs when we are not aiming for max speed (i.e. in turns or when braking)
			uint32_t ledState = alt_read_word(CarLedsAddress);
			if(speedSetting < MaxSpeed) {
				alt_write_word(CarLedsAddress, ledState & ~(1 << CAR_LED_HEADLIGHT_SHIFT));
			} else {
				alt_write_word(CarLedsAddress, ledState | (1 << CAR_LED_HEADLIGHT_SHIFT));
			}
			Controller::SetSetpoint(speedSetting);
		}

		//int pwmValue = Controller::GetOutput();
		//printf("%.5f; %.5f; %.5f; %.5f; %.5f; %.5f; %5d\n", dataSample.time, dataSample.distance - lapStartDistance, dataSample.speed, dataSample.yawRate, distanceCorrected, Controller::GetSetpoint(), pwmValue);
	}
}

timespec TimeDifference(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = BILLION+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}
