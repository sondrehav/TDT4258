#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#include "ex2.h"
#include "sound_generator.h"
#include "sound_player.h"

/* Declaration of peripheral setup functions */
void setupGPIO();
void setupTimer(uint32_t period);
void setupDAC();

/* Extern variables and function found in songs.c */
extern void audioSetup();
extern audio_t audio;
extern uint time;

/* Select what song to play based on what button is pressed. */
void selectSong();

int main(void)
{
	/* Call the peripheral setup functions */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);

	audioSetup();

	// Polling loop
	uint lastTimerValue = 0;
	uint gpioOld = 0;

	while (1) {
		uint timerValue = *TIMER1_CNT;

		// Check if new data should be pushed to the DAC.
		if (timerValue <= 150 && lastTimerValue > 150) {

			/* Check for change in button state. */
			if (*GPIO_PC_DIN != gpioOld) {
				gpioOld = *GPIO_PC_DIN;
				selectSong();
			}
			playAudio(&audio, time);
			time++;
		}
		lastTimerValue = timerValue;
	}

	return 0;
}

void selectSong()
{
	if (((~*GPIO_PC_DIN) & 0x1) == 0x1) {
		audio.sounds[0].state = Running;
	} else if (((~*GPIO_PC_DIN) & 0x2) == 0x2) {
		audio.sounds[1].state = Running;
	} else if (((~*GPIO_PC_DIN) & 0x4) == 0x4) {
		audio.sounds[2].state = Running;
	} else if (((~*GPIO_PC_DIN) & 0x8) == 0x8) {
		audio.sounds[3].state = Running;
	}
}
