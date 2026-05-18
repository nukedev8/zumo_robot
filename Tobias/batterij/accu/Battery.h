#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>


/*!
Klasse voor het uitlezen van de batterijspanning op de Zumo.
8-05-2026
*/


class Battery {
public:
/*!
gebruikt pin 1 om het voltage op te halen van de Zumo.
*lowThresholdMv is de waarde waaronder de batterij de zumo niet van genoeg power kan voorzien
*               standaard is dit 4600mV.
*/
    explicit Battery(uint8_t pin = 1, int lowThresholdMv = 4600);
    ~Battery() = default;

    /*!
    Leest de ruwe ADC waarde uit (0-1023)
    */
    uint16_t readRaw() const;
    /*!
    Berekent de batterijspanning in millivolts.
    mV = raw * 5000 * 2 / 1023
    de 2 is nodig vanwege de spanningsdeler op de Zumo.
    */
    uint16_t readMillivolts() const;

    /*!
    Geeft een true terug als de spanning onder de threshold komt.
    */
    bool isLow() const;

private:
    uint8_t _pin; // pinnummer voor de batterijmeting.
    int _lowThresholdMv; // Drempelwaarde in mV voor de lege batterij.
};

#endif