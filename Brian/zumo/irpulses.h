#ifndef IRPULSES_H
#define IRPULSES_H

#include <Zumo32U4.h>

/**
 * Reads the front IR sensor.
 */
class IRPulses {
public:
  /**
   * Creates an IRPulses object.
   * Constructor, no return.
   */
  IRPulses();

  /**
   * Prepares the front IR sensor.
   * Return type: void.
   */
  void begin();

  /**
   * Reads the newest IR pulse values.
   * Return type: void.
   */
  void read();

  /**
   * Gets the front sensor count when the left IR LEDs are used.
   * @return uint8_t, value from 0 to 6.
   */
  uint8_t frontLeftCount() const;

  /**
   * Gets the front sensor count when the right IR LEDs are used.
   * @return uint8_t, value from 0 to 6.
   */
  uint8_t frontRightCount() const;

  /**
   * Checks whether an object is detected in front of the Zumo.
   * @param threshold Minimum active readings needed, from 0 to 6.
   * @return bool, true when an object is detected, false when no object is detected.
   */
  bool objectInFront(uint8_t threshold = 6);

private:
  Zumo32U4ProximitySensors irPulsus;
};

#endif
