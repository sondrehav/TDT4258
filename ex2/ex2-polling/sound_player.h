# ifndef SOUND_PLAYER_H
# define SOUND_PLAYER_H

#include "sound_generator.h"
#include "ex2.h"
#include "efm32gg.h"

typedef enum SoundType {
	Saw, Triangle, Square
} soundType_t;

typedef enum PlayState 
{
	Running, Done, Paused
} playState_t;

typedef struct Song
{
	uint* song;
	uint length;
	uint tempo;
	bool looping;
} song_t;

typedef struct SoundPlayer 
{
	song_t* song;
	playState_t state;
	uint noteIndex;
	uint noteCounter;
	uint notePeriod;
	soundType_t soundType;
} soundPlayer_t;

typedef struct Audio
{
	soundPlayer_t* sounds;
	uint soundCount;
	uint volume;
} audio_t;

void initSoundPlayer(soundPlayer_t* player, song_t* song, soundType_t soundType);
fp playSong(soundPlayer_t* player, uint time);
void playAudio(audio_t* audio, uint time);

#endif