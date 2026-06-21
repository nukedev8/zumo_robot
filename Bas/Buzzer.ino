#include "Buzzer.h"

/**
 * @brief Constructor – initialiseert de buzzer.
 */
Buzzer::Buzzer() {}

/**
 * @brief Speelt een toon af op een gegeven frequentie.
 *
 * @param freqHz     Frequentie in Hertz.
 * @param durationMs Duur van de toon in milliseconden.
 * @param volume     Volume van 0 t/m 15.
 */
void Buzzer::playFrequency(int freqHz, int durationMs, int volume) {
    _buzzer.playFrequency(freqHz, durationMs, volume);
}

/**
 * @brief Stopt het afspelen van de huidige toon.
 */
void Buzzer::stop() {
    _buzzer.stopPlaying();
}

/**
 * @brief Geeft aan of de buzzer op dit moment actief is.
 *
 * @return true  Als de buzzer nog speelt.
 * @return false Als de buzzer gestopt is.
 */
bool Buzzer::isPlaying() const {
    return _buzzer.isPlaying();
}