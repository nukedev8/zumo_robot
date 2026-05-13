#include "irpulses.h"

/*
 * Creates an IRPulses object.
 * Constructor, no return.
 */
IRPulses::IRPulses() {}

/*
 * Prepares the front IR sensor.
 * Return type: void.
 */
void IRPulses::begin() {
  irPulsus.initFrontSensor();
}

/*
 * Reads the newest IR pulse values.
 * Return type: void.
 */
void IRPulses::read() {
  irPulsus.read();
}

/*
 * Gets the front sensor count when the left IR LEDs are used.
 * @return uint8_t, value from 0 to 6.
 */
uint8_t IRPulses::frontLeftCount() const {
  return irPulsus.countsFrontWithLeftLeds();
}

/*
 * Gets the front sensor count when the right IR LEDs are used.
 * @return uint8_t, value from 0 to 6.
 */
uint8_t IRPulses::frontRightCount() const {
  return irPulsus.countsFrontWithRightLeds();
}

/*
 * Checks if a close object is in front of the Zumo.
 * @param threshold Minimum active readings needed, from 0 to 6.
 * @return bool. True when an object is detected, false when no object is detected.
 */
bool IRPulses::objectInFront(uint8_t threshold) {
  read();

  return frontLeftCount() >= threshold || frontRightCount() >= threshold;
}
