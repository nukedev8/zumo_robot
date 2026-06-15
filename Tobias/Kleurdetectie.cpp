#include <Arduino.h>
#include "Kleurdetectie.h"

Kleurdetectie::Kleurdetectie()
    : current(Color::Unknown),
      pending(Color::Unknown),
      stableCount(0),
      stableThreshold(8)
{
    zones[0] = zones[1] = zones[2] = Color::Unknown;
    // Default drempels:
    ranges[0][0] = 750; ranges[0][1] = 910;   // zwart
    ranges[1][0] = 995; ranges[1][1] = 990;    // grijs
    ranges[2][0] = 130; ranges[2][1] = 430;    // groen
    ranges[3][0] = 70;  ranges[3][1] = 120;    // bruin
}

void Kleurdetectie::update(const uint16_t* v) {
    zones[0] = classify((v[0] + v[1]) / 2);
    zones[1] = classify(v[2]);
    zones[2] = classify((v[3] + v[4]) / 2);
    
    // Stabiliteits-check op middenkleur
    Color n = zones[1];
    if (n == pending) {
        if (stableCount < 255) stableCount++;
        if (stableCount >= stableThreshold) current = n;
    } else {
        pending = n;
        stableCount = 1;
    }
}

Color Kleurdetectie::classify(uint16_t v) const {
    for (uint8_t i = 0; i < 4; i++) {
        if (v >= ranges[i][0] && v <= ranges[i][1]) {
            return (Color)i;
        }
    }
    return Color::Unknown;
}

void Kleurdetectie::setRange(Color c, uint16_t mn, uint16_t mx) {
    uint8_t i = (uint8_t)c;
    if (i < 4) {
        ranges[i][0] = mn;
        ranges[i][1] = mx;
    }
}