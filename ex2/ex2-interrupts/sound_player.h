# ifndef SOUND_PLAYER_H
# define SOUND_PLAYER_H

enum SoundType {
	Saw, Triangle, Square
};

enum PlayState 
{
	Running, Done, Paused
};

typedef struct Song
{
	uint* song;
	uint length;
	uint tempo;
	bool looping;
} song_t;

typedef struct SoundPlayer 
{
	Song* song;
	PlayState state;
	uint noteIndex;
	uint noteCounter;
	uint notePeriod;
	SoundType soundType;
} soundPlayer_t;

typedef struct Audio
{
	SoundPlayer* sounds;
	uint soundCount;
	uint volume;
} audio_t;

#endif