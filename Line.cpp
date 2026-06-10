#include "Line.h"

/**
 * Creates the Line object and sets the starting sensor values to 0.
 * @param brownLineMinValue Lowest value that still counts as the brown line.
 * @param brownLineMaxValue Highest value that still counts as the brown line.
 * @param blackLineValue Value from which a black line is detected.
 * @param brownLineSensorsNeeded Number of sensors that must see the brown line.
 * @return Constructor has no return value.
 */
Line::Line(uint16_t brownLineMinValue,
           uint16_t brownLineMaxValue,
           uint16_t blackLineValue,
           uint8_t brownLineSensorsNeeded) {
  brownLineMinThreshold = brownLineMinValue;
  brownLineMaxThreshold = brownLineMaxValue;
  blackLineThreshold = blackLineValue;
  brownLineSensorCount = brownLineSensorsNeeded;

  for (uint8_t i = 0; i < sensorCount; i++) {
    sensorValues[i] = 0;
  }
}

/**
 * Prepares the five line sensors for use.
 * @return void
 */
void Line::begin() {
  lineSensors.initFiveSensors();
}

/**
 * Reads the newest values from the line sensors.
 * @return void
 */
void Line::read() {
  lineSensors.read(sensorValues);
}

/**
 * Checks if at least one sensor sees a black line.
 * @return bool True if a black line is detected, otherwise false.
 */
bool Line::seesBlackLine() {
  for (uint8_t i = 0; i < sensorCount; i++) {
    if (sensorValues[i] >= blackLineThreshold) {
      return true;
    }
  }

  return false;
}

/**
 * Checks if enough sensors see the brown line at the same time.
 * @return bool True if the brown line is detected, otherwise false.
 */
bool Line::crossedBrownLine() {
  uint8_t sensorsOnBrownLine = 0;
  uint8_t sensorsOnBlackLine = 0;

  for (uint8_t i = 0; i < sensorCount; i++) {
    if (sensorValues[i] >= blackLineThreshold) {
      sensorsOnBlackLine++;
    }

    if (sensorValues[i] >= brownLineMinThreshold &&
        sensorValues[i] <= brownLineMaxThreshold) {
      sensorsOnBrownLine++;
    }
  }

  if (sensorsOnBlackLine > 2) {
    return false;
  }

  if (sensorsOnBrownLine >= brownLineSensorCount) {
    return true;
  }

  return false;
}

/**
 * Calculates the position of the black line under the sensors.
 * @return int Position from 0 left to 4000 right, 2000 is center.
 */
int Line::getLinePosition() {
  long total = 0;
  long weightedTotal = 0;

  for (uint8_t i = 0; i < sensorCount; i++) {
    if (sensorValues[i] >= blackLineThreshold) {
      total = total + sensorValues[i];
      weightedTotal = weightedTotal + (long)sensorValues[i] * i * 1000;
    }
  }

  if (total == 0) {
    return 2000;
  }

  return weightedTotal / total;
}

/**
 * Returns the last measured value from one line sensor.
 * @param sensorNumber Sensor number, from 0 to 4.
 * @return uint16_t Sensor value, or 0 if the number does not exist.
 */
uint16_t Line::getValue(uint8_t sensorNumber) {
  if (sensorNumber >= sensorCount) {
    return 0;
  }

  return sensorValues[sensorNumber];
}
