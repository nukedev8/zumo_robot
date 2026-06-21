#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

#include <Arduino.h>

/**
 * @brief Beheert de stroombron detectie van de Zumo32U4.
 */
class PowerManager {
public:
    /**
     * @brief Controleert of de Zumo op USB-stroom loopt.
     *
     * @return true  Als USB aangesloten is.
     * @return false Als USB niet aangesloten is.
     */
    bool USBPowerPresent();

    /**
     * @brief Controleert of de Zumo op batterijstroom loopt.
     *
     * @return true  Als de batterij de stroombron is.
     * @return false Als USB de stroombron is.
     */
    bool batteryPowerPresent();
};

#endif