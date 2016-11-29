# Simulation Track Model

Um das zu verwendende neuronale Netzwerk etwas einzulernen, wird eine Simulation der Strecke ben�tigt.
Diese Ma�nahmen werden ergriffen, um andauernde Crashes w�hrend des Lernvorganges zu vermeiden.

Die Simulationsumgebung beinhaltet folgende Einheiten:
* Eine Strecke, welche aus kleinen Streckenst�cken zusammengebaut wird. Diese einzelnen 
	Streckenst�cke beinhalten Informationen bez�glich:
	* Winkel
	* Streckenl�nge
	* Typ (L | S | R) (Left | Straight | Right)
	* Startpunkt (x,y)
	Dabei ist der Endpunkt der Startpunkt des nachfolgenden Elements. Dieser wird beim Zusammensetzen
	der Strecke berechnet. Der Endpunkt des letzten Streckenst�ckes muss den Startpunkt des ersten Streckenst�ckes
	ergeben, damit eine vollst�ndige Strecke zusammengabaut wurde.
* Berechnung der Winkelgeschwindigkeit (Gyro-Wert, Drehung um Z-Achse)
* Berechnung der Zentrifugalkraft und anschlie�ender Vergleich mit festgelegter Maximalkraft um einen Crash zu simulieren.
* Berechnung der Drehzahl aus der Geschwindigkeit und dem Reifenradius.
	* durch die M�glichkeit die Drehzahl von ausserhalb zu ver�ndern kann ein Durchdrehen der Reifen simuliert werden.
		Das Durchdrehen f�hrt zu einem Positionsmismatch (Herausfinden, wie gut das neuronale Netzwerk diese Mismatches behandelt)


Die mathematischen Herleitungen f�r die Berechnungen k�nnen in dem Dokument [Math.pdf](link-to-math.pdf)) aufgefunden werden.