#include "Battery.h"

Battery::Battery(uint8_t pin, int lowThresholdMv)
    : _pin(pin), _lowThresholdMv(lowThresholdMv)
{
}

uint16_t Battery::readRaw() const {
    return analogRead(_pin);
}

uint16_t Battery::readMillivolts() const {
    long raw = analogRead(_pin);
    long v_adc_mv = raw * 5000L / 1023L;
    return (uint16_t)(v_adc_mv * 2);
}

bool Battery::isLow() const {
    return readMillivolts() < _lowThresholdMv;
}