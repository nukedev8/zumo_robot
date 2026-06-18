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
    /**
     * Maakt een Kleurdetectie object aan met standaard kleurgrenzen.
     * @return Constructor heeft geen returnwaarde.
     */
    Kleurdetectie();

    /**
     * Werkt de gedetecteerde kleur bij met de nieuwste lijnsensorwaarden.
     * @param values Pointer naar de array met vijf lijnsensorwaarden.
     * @return void
     */
    void update(const uint16_t* values);

    /**
     * Geeft de stabiel herkende kleur terug.
     * @return Color Huidige stabiele kleur.
     */
    Color getColor() const { return current; }

    /**
     * Geeft de kleur van een zone terug.
     * @param z Zone nummer: 0 is links, 1 is midden, 2 is rechts.
     * @return Color Kleur van de gekozen zone, of Unknown bij een verkeerd nummer.
     */
    Color getZone(uint8_t z) const { return (z < 3) ? zones[z] : Color::Unknown; }

    /**
     * Past de minimum- en maximumwaarde van een kleur aan.
     * @param c Kleur waarvan de grens aangepast moet worden.
     * @param mn Laagste waarde die bij deze kleur hoort.
     * @param mx Hoogste waarde die bij deze kleur hoort.
     * @return void
     */
    void setRange(Color c, uint16_t mn, uint16_t mx);

    /**
     * Zet hoeveel metingen dezelfde kleur moeten geven voor stabiliteit.
     * @param frames Aantal metingen dat nodig is.
     * @return void
     */
    void setStability(uint8_t frames) { stableThreshold = frames; }

private:
    Color    current;
    Color    pending;
    Color    zones[3];               // 0=links, 1=midden, 2=rechts
    uint8_t  stableCount;
    uint8_t  stableThreshold;
    uint16_t ranges[4][2];           // [Color][0=min, 1=max]
    
    /**
     * Bepaalt welke kleur bij een sensorwaarde hoort.
     * @param v Sensorwaarde die gecontroleerd wordt.
     * @return Color Herkende kleur, of Unknown als er geen match is.
     */
    Color classify(uint16_t v) const;
};

#endif
