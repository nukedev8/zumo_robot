#ifndef LINESENSOR_H
#define LINESENSOR_H

#include <Arduino.h>

/*!
Klasse voor het uitlezen van de lijnsensoren op de Zumo.
8-05-2026
*/

/*!
Max wachtwijd in microseconden voor de RC-ontlading
*/
#define SENSOR_TIMEOUT_US 2500

/*!
waarden boven de 500 worden als zwart aangezien.
*/
#define SENSOR_BLACK_THRESHOLD 500

/*!
Om later acties uit te voeren bij de grijze lijn.
*/
//#define Sensor_grey_Threshold

/*!
Om later acties uit te voeren bij de bruine lijn.
*/
//#define Sensor_Brown_Threshold 

/*!
@class LineSensor

De sensor werkt door een condensator op te laden via de I/0-pin
en dan te meten hoelang het duurt voordat de pin laag wordt.
Meer IR-reflectie (wit) = snelle ontlading > lage waarde.
minder reflectie duid juist op zwart waardoor de waarde hoog is.
*/
class LineSensor {
public:

/*!
pinnummer van de sensoren
*/
    LineSensor(uint8_t pin);
/*!
Maakt een kopie van de sensor
*/
    LineSensor(const LineSensor& other);

    ~LineSensor() = default;
/*!
Meet de ontlaadtijd van de sensorpin in microseconden
*/
    uint16_t read();
/*!
De functie voor het teruggeven van true als de sensor de zwarte lijn ziet.
*/
    bool OnBlack();
/*!
Geeft true terug bij het tegenkomen van de grijze lijn.
*/
    bool OnGrey();
/*!
Geeft true terug bij het tegenkomen van de grijze lijn.
*/
    bool OnBrown();


private:
    uint8_t _pin; // pinnummer van de sensor.
};
/*!
De klasse met de 5 lijnsensoren (DN1-DN5).
De IR-emitters worden bestuurd via pin 11.
*/

class LineSensorArray {
public:

/*!
Constructor voor de vijf sensoren.
*/
    LineSensorArray();

    ~LineSensorArray() = default;

/*!
Zet de emitters aan en wacht 1 milliseconden.
*/
    void init();

/*!
Zet de emitters aan, omdat de sensoren anders niet werken.
*/
    void emitterOn();
/*!
Als de sensoren niet worden gebruikt gaan de emitters uit, om stroom te besparen.
*/
    void emitterOff();
/*!
Leest de sensoren uit en slaat de waarden op.
*/
    void readAll(uint16_t values[5]);

private:
    LineSensor _sensors[5]; // De 5 sensoren.
    static const uint8_t EMITTER_PIN = 11; // De pin om de Emitter aan te sturen.
};

#endif