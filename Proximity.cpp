#include "Proximity.h"

/**
 * Maakt een Proximity object aan en bewaart de drempelwaarden.
 * @param detectValue Waarde vanaf waar een object wordt gezien.
 * @param closeValue Waarde vanaf waar een object dichtbij is.
 * @param centerDifference Maximaal verschil tussen links en rechts voor midden.
 * @return Constructor heeft geen returnwaarde.
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
 * Zet de proximity sensor aan de voorkant klaar voor gebruik.
 * @return void
 */
void Proximity::begin() {
  proxSensors.initFrontSensor();
}

/**
 * Leest de nieuwste proximity waarden uit.
 * @return void
 */
void Proximity::read() {
  proxSensors.read();
  leftValue = proxSensors.countsFrontWithLeftLeds();
  rightValue = proxSensors.countsFrontWithRightLeds();
}

/**
 * Controleert of beide sensoren een object zien.
 * @return bool True als links en rechts boven de detectiewaarde zitten.
 */
bool Proximity::objectDetected() {
  if (leftValue >= detectThreshold && rightValue >= detectThreshold) {
    return true;
  }

  return false;
}

/**
 * Controleert of het object dichtbij genoeg is.
 * @return bool True als links en rechts boven de dichtbijwaarde zitten.
 */
bool Proximity::objectClose() {
  if (leftValue >= closeThreshold && rightValue >= closeThreshold) {
    return true;
  }

  return false;
}

/**
 * Bepaalt waar het object staat.
 * @return ObjectDirection Richting van het object: None, Left, Right of Center.
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
 * Geeft de laatste linker proximity waarde terug.
 * @return uint8_t Linker proximity waarde.
 */
uint8_t Proximity::getLeftValue() {
  return leftValue;
}

/**
 * Geeft de laatste rechter proximity waarde terug.
 * @return uint8_t Rechter proximity waarde.
 */
uint8_t Proximity::getRightValue() {
  return rightValue;
}
