#include "Proximity.h"

/**
 * Creates the Proximity object and stores the sensor thresholds.
 * @param detectValue Value from which an object is detected.
 * @param closeValue Value from which an object is close.
 * @param centerDifference Maximum difference between left and right for center.
 * @return Constructor has no return value.
 */
Proximity::Proximity(uint8_t detectValue,
                     uint8_t closeValue,
                     uint8_t centerDifference) {
  leftValue = 0;
  rightValue = 0;
  detectThreshold = detectValue;
  closeThreshold = closeValue;
  centerMargin = centerDifference;
}

/**
 * Prepares the front proximity sensor for use.
 * @return void
 */
void Proximity::begin() {
  proxSensors.initFrontSensor();
}

/**
 * Reads the newest proximity values.
 * @return void
 */
void Proximity::read() {
  proxSensors.read();
  leftValue = proxSensors.countsFrontWithLeftLeds();
  rightValue = proxSensors.countsFrontWithRightLeds();
}

/**
 * Checks if the sensor sees an object.
 * @return bool True if left and right are above the detect value.
 */
bool Proximity::objectDetected() {
  if (leftValue >= detectThreshold && rightValue >= detectThreshold) {
    return true;
  }

  return false;
}

/**
 * Checks if the object is close enough.
 * @return bool True if left and right are above the close value.
 */
bool Proximity::objectClose() {
  if (leftValue >= closeThreshold && rightValue >= closeThreshold) {
    return true;
  }

  return false;
}

/**
 * Determines where the object is.
 * @return ObjectDirection None, Left, Right of Center.
 */
ObjectDirection Proximity::getDirection() {
  uint8_t difference = 0;

  if (!objectDetected()) {
    return ObjectDirection::None;
  }

  if (leftValue > rightValue) {
    difference = leftValue - rightValue;
  } else {
    difference = rightValue - leftValue;
  }

  if (difference <= centerMargin) {
    return ObjectDirection::Center;
  }

  if (leftValue > rightValue) {
    return ObjectDirection::Left;
  }

  return ObjectDirection::Right;
}

/**
 * Returns the last left proximity value.
 * @return uint8_t Left proximity value.
 */
uint8_t Proximity::getLeftValue() {
  return leftValue;
}

/**
 * Returns the last right proximity value.
 * @return uint8_t Right proximity value.
 */
uint8_t Proximity::getRightValue() {
  return rightValue;
}
