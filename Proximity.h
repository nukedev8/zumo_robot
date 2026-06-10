#ifndef promixity_h
#define promixity_h

#include <Arduino.h>
#include <Zumo32U4.h>

enum class ObjectDirection {
  None,
  Left,
  Right,
  Center
};

class Proximity {
public:
  /**
   * Creates the Proximity object and stores the sensor thresholds.
   * @param detectValue Value from which an object is detected.
   * @param closeValue Value from which an object is close.
   * @param centerDifference Maximum difference between left and right for center.
   * @return Constructor has no return value.
   */
  Proximity(uint8_t detectValue = 2,
            uint8_t closeValue = 5,
            uint8_t centerDifference = 1);

  /**
   * Prepares the front proximity sensor for use.
   * @return void
   */
  void begin();

  /**
   * Reads the newest proximity values.
   * @return void
   */
  void read();

  /**
   * Checks if the sensor sees an object.
   * @return bool True if left and right are above the detect value.
   */
  bool objectDetected();

  /**
   * Checks if the object is close enough.
   * @return bool True if left and right are above the close value.
   */
  bool objectClose();

  /**
   * Determines where the object is.
   * @return ObjectDirection None, Left, Right of Center.
   */
  ObjectDirection getDirection();

  /**
   * Returns the last left proximity value.
   * @return uint8_t Left proximity value.
   */
  uint8_t getLeftValue();

  /**
   * Returns the last right proximity value.
   * @return uint8_t Right proximity value.
   */
  uint8_t getRightValue();

private:
  Zumo32U4ProximitySensors proxSensors;

  uint8_t leftValue;
  uint8_t rightValue;
  uint8_t detectThreshold;
  uint8_t closeThreshold;
  uint8_t centerMargin;
};

#endif
