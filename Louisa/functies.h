#ifndef FUNCTIES_H
#define FUNCTIES_H

#include <Zumo32U4.h>

class AfstandsSensoren {

private:  
Zumo32U4ProximitySensors proxSensors;  
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;

bool motoren_aan = false;  //bool voor het aan en uit zetten van de motoren met de knop
const uint8_t objectVoorSensor = 2;  //waarde waarmee de Zumo kan zien dat er iets voor de zenzor zit
const uint16_t draaiSnelheid = 180;  //snelheid voor het draaien van de Zumo, max is 400
const uint16_t vooruit = 120;  //snelheid van het vooruitrijden van de Zumo, max is 400


public:
~AfstandsSensoren();  //constructor en deconstructor
AfstandsSensoren();

// voids die in de cpp staan uitgeschreven
void init();
void aanKnop();

void draaiRechts();
void draaiLinks();
void stop();
void start();

};

#endif // FUNCTIES_H
