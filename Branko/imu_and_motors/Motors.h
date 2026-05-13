#ifndef MOTORS_H
#define MOTORS_H

#include <Zumo32U4.h>

class Motors {
    Zumo32U4Encoders encoders;
    Zumo32U4Motors motors;

public:
    /*! \brief
     * Move forward or backward.
     * \param distance amount of encoder units to travel
     */
    void move(int16_t distance);

    /*! \brief
     * Move each track forward or backward separately
     * \param left_distance amount of encoder units for left track to travel
     * \param right_distance amount of encoder units for right track to travel
     */
    void move(int16_t left_distance, int16_t right_distance);

};

#endif // MOTORS_H
