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

uint songData[] = {
	48, 50, 52, 53, 55, 55, 55, 55, 57, 57, 57, 57, 
	55, 55, 55, 55, 53, 53, 53, 53, 52, 52, 52, 52, 
	50, 50, 50, 50, 48, 48, 48, 48
};

uint songData2[] = {
	48, 52, 55, 60, 64, 55, 60, 64,
	48, 52, 55, 60, 64, 55, 60, 64,
	48, 50, 55, 62, 65, 55, 62, 65, 
	48, 50, 55, 62, 65, 55, 62, 65, 
	47, 50, 55, 61, 65, 55, 61, 65, 
	47, 50, 55, 61, 65, 55, 61, 65, 
	48, 52, 55, 60, 64, 55, 60, 64,
	48, 52, 55, 60, 64, 55, 60, 64
};

uint songData3[] = {
	0, 0, 50
};

uint songData4[] = {
	100, 90, 80, 70, 60, 50, 40, 30, 20, 10, 0
};

int main(void)
{
	/* Call the peripheral setup functions */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);
	
	song_t songs[4];
	soundPlayer_t soundPlayers[4];
	audio_t audio = {soundPlayers, 4, 4};
	
	song_t song1 = {songData, 32, 120, 0};
	song_t song2 = {songData2, 64, 240, 0};
	song_t song3 = {songData3, 3, 60, 1};
	song_t song4 = {songData4, 11, 120, 1};
	songs[0] = song1;
	songs[1] = song2;
	songs[2] = song3;
	songs[3] = song4;
	
	initSoundPlayer(soundPlayers, songs, Saw);
	initSoundPlayer(soundPlayers+1, songs+1, Saw);
	initSoundPlayer(soundPlayers+2, songs+2, Saw);
	initSoundPlayer(soundPlayers+3, songs+3, Saw);
	
	// Polling loop
	uint count = 0; 
	uint lastTimerValue = 0;
	
	while (1) {
		uint timerValue = *TIMER1_CNT;
		
		// Check if new data should be pushed to the DAC.
		if (timerValue <= 150 && lastTimerValue > 150) {
			
			playAudio(&audio, count);
			
			if (count == 0xffffffff) count = 0;
			else count++;
		}
		lastTimerValue = timerValue;
	}

	return 0;
}