#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#include "ex2.h"
#include "sound_generator.h"
#include "sound_player.h"

/* Sound data and variables. */

uint songStartup[] = {
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
   64, 67, 76, 72, 74, 79
};

uint songData1[] = {
	48, 50, 52, 53, 55, 55, 55, 55, 57, 57, 57, 57, 
	55, 55, 55, 55, 53, 53, 53, 53, 52, 52, 52, 52, 
	50, 50, 50, 50, 48, 48, 48, 48
};

uint songData2[] = {
	100, 90, 80, 70, 60, 50, 40, 30, 20, 10, 0
};

song_t song1 = {songStartup, 64, 480, 0};
song_t song2 = {song1up, 6, 360, 0};
song_t song3 = {songData1, 32, 240, 0};
song_t song4 = {songData2, 11, 120, 0};



song_t songs[4];
soundPlayer_t soundPlayers[4];
audio_t audio = {soundPlayers, 4};
uint time;



void audioSetup() {
	songs[0] = song1;
	songs[1] = song2;
	songs[2] = song3;
	songs[3] = song4;
	
	initSoundPlayer(soundPlayers, songs, Saw, 4);
	initSoundPlayer(soundPlayers+1, songs+1, Saw, 4);
	initSoundPlayer(soundPlayers+2, songs+2, Square, 4);
	initSoundPlayer(soundPlayers+3, songs+3, Saw, 4);
	
	soundPlayers[0].state = Paused;
	soundPlayers[1].state = Paused;
	soundPlayers[2].state = Paused;
	soundPlayers[3].state = Paused;
	
	time = 0;
}