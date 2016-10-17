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

uint bumb[] = {
	30
};

uint shot[] = {
	60, 64, 67, 72, 76, 67, 72, 76,
	60, 52, 55, 60, 64, 55, 60, 64,
	48, 38, 43, 50, 53, 43, 50, 53,
	24, 26, 31, 38, 41, 31, 38, 41
};

song_t song1 = { songStartup, 64, 480, 0 };
song_t song2 = { song1up, 6, 360, 0 };
song_t song3 = { bumb, 1, 240, 0 };
song_t song4 = { shot, 32, 6000, 0 };

song_t songs[4];
soundPlayer_t soundPlayers[4];
audio_t audio = { soundPlayers, 4 };

uint time;
uint runningCount;
bool noAudio;

void audioSetup()
{
	songs[0] = song1;
	songs[1] = song2;
	songs[2] = song3;
	songs[3] = song4;

	initSoundPlayer(soundPlayers, songs, Square, 4);
	initSoundPlayer(soundPlayers + 1, songs + 1, Square, 4);
	initSoundPlayer(soundPlayers + 2, songs + 2, Square, 4);
	initSoundPlayer(soundPlayers + 3, songs + 3, Square, 4);

	soundPlayers[0].state = Paused;
	soundPlayers[1].state = Paused;
	soundPlayers[2].state = Paused;
	soundPlayers[3].state = Paused;

	time = 0;
	runningCount = 0;
	noAudio = 1;
}
