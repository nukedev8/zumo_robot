#include "PowerManager.h"

/**
 * @brief Controleert of de Zumo op USB-stroom loopt.
 *
 * Leest het VBUS-bit uit het USBSTA-register:
 * - 1 = USB aangesloten
 * - 0 = batterij is de stroombron
 *
 * @return true  Als USB aangesloten is.
 * @return false Als USB niet aangesloten is.
 */
bool PowerManager::USBPowerPresent() {
    bool USBAanwezig = bitRead(USBSTA, VBUS);
    return USBAanwezig;
}

/**
 * @brief Controleert of de Zumo op batterijstroom loopt.
 *
 * Geeft het omgekeerde terug van USBPowerPresent().
 *
 * @return true  Als de batterij de stroombron is.
 * @return false Als USB de stroombron is.
 */
bool PowerManager::batteryPowerPresent() {
    return !USBPowerPresent();
}