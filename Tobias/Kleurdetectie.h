#ifndef Kleurdetectie_H
#define Kleurdetectie_H

#include <Arduino.h>

/**
* klasse voor het bepalen welke kleur bij welke waarde hoort.
*/

enum class Color {
    Black = 0, // zwart, op volle snelheid de baan volgen
    Grey  = 1, // grijs, voor de splitsingen
    Green = 2, // groen, op halve snelheid de baan volgen
    Brown = 3, // bruin, voor het zoeken van het blok
    Unknown
};

class Kleurdetectie {
public:
    Kleurdetectie();
    
    void update(const uint16_t* values);
    Color getColor() const { return current; }
    Color getZone(uint8_t z) const { return (z < 3) ? zones[z] : Color::Unknown; }
    
    // mn=min, mx=max. Voor Black wordt alleen mn gebruikt (= drempel).
    void setRange(Color c, uint16_t mn, uint16_t mx);
    void setStability(uint8_t frames) { stableThreshold = frames; }

private:
    Color    current;
    Color    pending;
    Color    zones[3];               // 0=links, 1=midden, 2=rechts
    uint8_t  stableCount;
    uint8_t  stableThreshold;
    uint16_t ranges[4][2];           // [Color][0=min, 1=max]
    
    Color classify(uint16_t v) const;
};

#endif