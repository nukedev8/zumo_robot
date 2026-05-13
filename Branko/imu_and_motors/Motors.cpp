#include "Motors.h"

/*! \brief
 * Compare ints like in qsort.
 * \retval 1 if a > b
 * \retval 0 if a == b
 * \retval -1 if a < b
 */
static int8_t cmp(int a, int b) {
    if (a > b)
        return 1;
    if (a == b)
        return 0;
    return -1;
}

/*! \brief
 * Get the sign of a number.
 * \retval 1 if positive
 * \retval 0 if zero
 * \retval -1 if negative
 */
static int8_t sign(int n) {
    return cmp(n, 0);
}

void Motors::move(int16_t distance) {
    move(distance, distance);
}

void Motors::move(int16_t left_distance, int16_t right_distance) {
    // Reset counters as a precaution
    encoders.getCountsAndResetLeft();
    encoders.getCountsAndResetRight();

    while (true) {
        int16_t left_travelled = encoders.getCountsAndResetLeft();
        int16_t right_travelled = encoders.getCountsAndResetRight();

        // Check for overshoots
        if (abs(left_travelled) > abs(left_distance))
            left_distance = 0;
        else
            left_distance -= left_travelled;

        if (abs(right_travelled) > abs(right_distance))
            right_distance = 0;
        else
            right_distance -= right_travelled;

        // Speed is 400, -400 or 0 depending on which direction to go
        int16_t left_speed = 400 * sign(left_distance);
        int16_t right_speed = 400 * sign(right_distance);

        motors.setSpeeds(left_speed, right_speed);
        if ((left_distance == 0) && (right_distance == 0))
            break;
    }
}
