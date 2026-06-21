#ifndef BUZZER_H
#define BUZZER_H

#include <Zumo32U4.h>

/**
 * @brief Beheert de buzzer van de Zumo32U4.
 */
class Buzzer {
public:
    /**
     * @brief Constructor – initialiseert de buzzer.
     */
    Buzzer();

    /**
     * @brief Speelt een toon af op een gegeven frequentie.
     *
     * @param freqHz     Frequentie in Hertz.
     * @param durationMs Duur van de toon in milliseconden.
     * @param volume     Volume van 0 t/m 15 (standaard 15).
     */
    void playFrequency(int freqHz, int durationMs, int volume = 15);

    /**
     * @brief Stopt het afspelen van de huidige toon.
     */
    void stop();

    /**
     * @brief Geeft aan of de buzzer op dit moment actief is.
     *
     * @return true  Als de buzzer nog speelt.
     * @return false Als de buzzer gestopt is.
     */
    bool isPlaying() const;

private:
    Zumo32U4Buzzer _buzzer; ///< De onderliggende Zumo buzzer driver.
};

#endif