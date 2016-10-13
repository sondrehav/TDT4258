#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#include "sound_player.h"



/* Declaration of peripheral setup functions */
void setupGPIO();
void setupTimer(uint32_t period);
void setupDAC();

uint songData[] = {
	48, 50, 52, 53, 55, 55, 55, 55, 57, 57, 57, 57, 
	55, 55, 55, 55, 53, 53, 53, 53, 52, 52, 52, 52, 
	50, 50, 50, 50, 48, 48, 48, 48
};
Song_t song = {songData, 32, 120, 1};
SoundPlayer player;
initSoundPlayer(player, &song, SoundType.Saw);
Audio audio = {&player, 1, 128};

int main(void)
{
	/* Call the peripheral setup functions */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);
	
	// Polling loop
	uint count = 0; 
	uint lastTimerValue = 0;
	
	while (1) {
		uint timerValue = *TIMER1_CNT;
		
		// Check if new data should be pushed to the DAC.
		if (timerValue <= 150 && lastTimerValue > 150) {
			playBeat(beat_index, count);
			
			playAudio(&audio, count);
			
			if (count == 0xffffffff) count = 0;
			else count++;
		}
		lastTimerValue = timerValue;
	}

	return 0;
}