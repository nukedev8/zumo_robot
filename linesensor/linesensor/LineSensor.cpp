#include "LineSensor.h"

/*!
Implementatie van de lijnsensor.
8-05-2026
*/
LineSensor::LineSensor(uint8_t pin)
    : _pin(pin)
{
}

LineSensor::LineSensor(const LineSensor& other)
    : _pin(other._pin)
{
}

uint16_t LineSensor::read() {
    // Laad condensator op door pin omhoog te zetten.
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, HIGH);
    delayMicroseconds(10);

    // Schakelt naar input zodat de condensator kan ontladen.
    pinMode(_pin, INPUT);

    // telt het aantal microseconden totdat de pin laag wordt of de timeout is bereikt.
    uint16_t count = 0;
    while (digitalRead(_pin) == HIGH && count < SENSOR_TIMEOUT_US) {
        count++;
        delayMicroseconds(1);
    }
    return count;
}

bool LineSensor::OnBlack() {
    return read() > SENSOR_BLACK_THRESHOLD;
}

bool LineSensor::OnGrey() {
    return read() > Sensor_grey_Threshold;
}

bool LineSensor::OnBrown() {
    return read() > Sensor_Brown_Threshold;
}

LineSensorArray::LineSensorArray()
    : _sensors{
        LineSensor(A0),   // DN1 kijkt naar links.
        LineSensor(A2),   // DN2 kijkt schuin naar links.
        LineSensor(A3),   // DN3 kijkt naar het midden.
        LineSensor(A6),   // DN4 kijkt schuin naar rechts.
        LineSensor(A11)   // DN5 kijkt naar rechts.
    }
{
}

void LineSensorArray::emitterOn() {
    pinMode(EMITTER_PIN, OUTPUT);
    digitalWrite(EMITTER_PIN, HIGH);
}

void LineSensorArray::emitterOff() {
    pinMode(EMITTER_PIN, OUTPUT);
    digitalWrite(EMITTER_PIN, LOW);
}

void LineSensorArray::init() {
    emitterOn();
    delay(1); // wacht 1 miliseconden zodat de emitters vol zijn.
}

void LineSensorArray::readAll(uint16_t values[5]) {
    for (uint8_t i = 0; i < 5; i++) {
        values[i] = _sensors[i].read();
    }
}