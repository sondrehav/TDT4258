#ifndef SOUND_PLAYER_H
#define SOUND_PLAYER_H

#include "sound_generator.h"
#include "ex2.h"
#include "efm32gg.h"

/* Waveform to use when playing sound. */
typedef enum SoundType {
	Saw, Triangle, Square
} soundType_t;

/* State storing if music should be played or not. */
typedef enum PlayState {
	Running, Done, Paused
} playState_t;

/* Stores a song. */
typedef struct Song {
	uint *song;
	uint length;
	uint tempo;
	bool looping;
} song_t;

/* Used for playing a sound. */
typedef struct SoundPlayer {
	song_t *song;
	playState_t state;
	uint noteIndex;
	uint noteCounter;
	uint notePeriod;
	soundType_t soundType;
	uint volume;
	 fp(*soundGen) (fp, uint);	// Store sound generation function, so we won't have to look it up during sampling.
} soundPlayer_t;

/* Used to play mulitple sounds. Should only be one of these in the program. */
typedef struct Audio {
	soundPlayer_t *sounds;
	uint soundCount;
} audio_t;

/* Used to initialize a sound player. Caller is responsible for allocating space. */
void initSoundPlayer(soundPlayer_t * player, song_t * song,
		     soundType_t soundType, uint volume);

/* Get next sample from song. */
fp playSong(soundPlayer_t * player, uint time);

/* Play songs. Call once per sampling period. Time is a clock variable mesured in sample rate ticks. */
void playAudio(audio_t * audio, uint time);

/* Make song start playing from the start. */
void restart(soundPlayer_t * player);

#endif
