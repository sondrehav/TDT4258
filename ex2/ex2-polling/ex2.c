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



void selectSong();

/* Sound data and variables. */
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

uint song1up[] = {
   52, 55, 64, 60, 62, 67
};

uint songData4[] = {
	100, 90, 80, 70, 60, 50, 40, 30, 20, 10, 0
};



song_t songs[4];
soundPlayer_t soundPlayers[4];
audio_t audio = {soundPlayers, 4};

song_t song1 = {songData, 32, 240, 0};
song_t song2 = {songData2, 64, 480, 0};
song_t song3 = {song1up, 6, 360, 0};
song_t song4 = {songData4, 11, 120, 0};

uint time;

int main(void)
{
	/* Call the peripheral setup functions */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);
	
	/* Audio setup. */
	songs[0] = song1;
	songs[1] = song2;
	songs[2] = song3;
	songs[3] = song4;
	
	initSoundPlayer(soundPlayers, songs, Saw, 4);
	initSoundPlayer(soundPlayers+1, songs+1, Saw, 4);
	initSoundPlayer(soundPlayers+2, songs+2, Saw, 4);
	initSoundPlayer(soundPlayers+3, songs+3, Saw, 4);
	
	soundPlayers[0].state = Paused;
	soundPlayers[1].state = Paused;
	soundPlayers[2].state = Paused;
	soundPlayers[3].state = Paused;
	
	time = 0;
	
	// Polling loop
	uint lastTimerValue = 0;
	uint gpioOld = 0;
	
	while (1) {
		uint timerValue = *TIMER1_CNT;
		
		// Check if new data should be pushed to the DAC.
		if (timerValue <= 150 && lastTimerValue > 150) {
			
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

void selectSong(){
	if (((~*GPIO_PC_DIN) & 0x1) == 0x1)
	{
		audio.sounds[0].state = Running;
	}
	else if (((~*GPIO_PC_DIN) & 0x2) == 0x2)
	{
		audio.sounds[1].state = Running;
	}
	else if (((~*GPIO_PC_DIN) & 0x4) == 0x4)
	{
		audio.sounds[2].state = Running;
	}
	else if (((~*GPIO_PC_DIN) & 0x8) == 0x8)
	{
		audio.sounds[3].state = Running;
	}
}