# Mapping Algorithmus

Die grunds�tzliche Idee besteht darin, zu erkennen, aus welchen Streckenelementen die Bahn zusammengesetzt ist und wie diese aneinandergereiht sind.
Dies kann �ber die Flankenerkennung des Gyro-/Beschleunigungssensor realisiert werden. Somit wei� man, ob man sich in einer Kurve oder auf einem
geraden Streckenst�ck befindet. Diese Information w�rde dann als Array abgespeichert werden. Somit k�nnte man die Strecke vermessen. Zus�tzlich 
zur Kartographierung der Strecke k�nnte �ber die Zeit, die bei dem Vermessungsdurchlauf �ber die jeweiligen Rechts-/Linkskurvenst�cke vergeht, herausgefunden 
werden, ob man sich auf der inneren oder der �u�eren Bahn befindet.

Durch die Tatsache, dass Carrera 4 verschiedene Kurvenradien mit jeweils verschiedenen L�ngen/Winkeln (15�, 30�, 45�, 60�, 90�) zur Verf�gung stellt, wird diese 
Erkennung um einiges schwieriger. Daher wird auf einen Algorithmus umgestellt, der den Radius und die zur�ckgelegte Strecke in der Kurve vermisst. Somit ist das 
Projekt auch auf zuk�nftige Streckenerweiterungen seitens Carrera gewappnet. Somit wird die Flankenerkennung beibehalten und der Rest �berarbeitet.

Links zu einigen Carrera Kurven: 
* 15� Kurven 
  * [Kurve 4](https://shop.carrera-toys.com/Carrera-DIGITAL-132/Zubehoer/Schienen/Kurven/Kurve-4-15-Grad.html?shp=2)
* 30� Kurven
  * [Kurve 1](https://shop.carrera-toys.com/Carrera-DIGITAL-132/Zubehoer/Schienen/Kurven/Kurve-1-30-Grad.html?shp=2)
  * [Kurve 2](https://shop.carrera-toys.com/Carrera-DIGITAL-132/Zubehoer/Schienen/Kurven/Kurve-2-30-Grad.html?shp=2)
  * [Kurve 3](https://shop.carrera-toys.com/Carrera-DIGITAL-132/Zubehoer/Schienen/Kurven/Kurve-3-30-Grad.html?shp=2)
* 45� Kurven
  * [Kurve 1](https://shop.carrera-toys.com/Carrera-GO/Zubehoer/Schienen/Kurve-1-45-Grad.html?shp=4)
  * [Kurve 2](https://shop.carrera-toys.com/Carrera-GO/Zubehoer/Schienen/Kurve-2-45-Grad.html?shp=4)
  * [Kurve 3](https://shop.carrera-toys.com/Carrera-GO/Zubehoer/Schienen/Kurve-3-45-Grad.html?shp=4)
* 60� Kurven
  * [Kurve 1](https://shop.carrera-toys.com/Carrera-DIGITAL-132/Zubehoer/Schienen/Kurven/Kurve-1-60-Grad.html?shp=2)
* 90� Kurven
  * [Kurve 1](https://shop.carrera-toys.com/Carrera-GO/Zubehoer/Schienen/Kurve-1-90-Grad.html?shp=4&lang=0&shp=4&searchparam=Kurve)