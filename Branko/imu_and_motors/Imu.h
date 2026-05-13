#ifndef IMU_H
#define IMU_H

#include "Vec.h"
#include <Zumo32U4.h>

class Imu {
    Zumo32U4IMU inner_imu;
    Vec<float> gyro_offset{};
    Vec<float> acc_offset{};
    Vec<float> heading{};
    Vec<float> vel{};
    Vec<float> pos{};
    int32_t last_update = 0;

public:
    /*! \brief
     * Initialize the IMU. Requires Wire.begin() to have been called first.
     * \retval 0 if successful
     * \retval 1 if unsuccessful
     */
    int init();

    /*! \brief
     * (re)calibrate the IMU to account for constant offset in sensor readings.
     */
    void calibrate();

    /*! \brief
     * Must be called inside loop() frequently to avoid drift.
     */
    void update();

    /*! \brief
     * Reset heading, velocity and position vectors back to 0.
     */
    void reset();

    /*! \brief
     * Return total heading change since calibration
     * started, in degrees.
     */
    Vec<float> getHeading() const;

    /*! \brief
     * Return measured velocity in m/s, not accounting for rotation.
     */
    Vec<float> getVelocity() const;

    /*! \brief
     * Return measured position in m/s, not accounting for rotation.
     */
    Vec<float> getPosition() const;
};

#endif // IMU_H
