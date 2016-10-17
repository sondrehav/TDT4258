#include "sound_player.h"
#include "sound_generator.h"
#include "ex2.h"

// Variables found in songs.c
extern uint runningCount;
extern bool noAudio;

void initSoundPlayer(soundPlayer_t* player, song_t* song, soundType_t soundType, uint volume) {
	player->song = song;
	player->state = Running;
	player->noteIndex = 0;
	player->noteCounter = 0;
	player->notePeriod = SAMPLE_RATE * 60 / song->tempo; // Calculate note period in sample rate ticks.
	player->soundType = soundType;
	player->volume = volume;
	switch(soundType) {
		case Saw:
			player->soundGen = sawWave;
			break;
		case Triangle:
			player->soundGen = triangleWave;
			break;
		case Square:
			player->soundGen = squareWave;
			break;
	}
}

fp playSong(soundPlayer_t* player, uint time) {
	fp freq = getFreqNote(player->song->song[player->noteIndex]); // Get frequency of current note.
	
	// Sample using specified waveform.
	fp sample = player->soundGen(freq, time);
	
	player->noteCounter++;
	if (player->noteCounter == player->notePeriod) { // End of note.
		player->noteIndex++;
		player->noteCounter = 0;
		if (player->noteIndex == player->song->length) { // End of song.
			player->noteIndex = 0;
			if (!player->song->looping) {
				player->state = Done;
				runningCount--;
				if (runningCount == 0) noAudio = 1;
			}
		}
	}
	sample *= player->volume; // Scale with specified volume
	return sample;
}

void playAudio(audio_t* audio, uint time) {
	fp sum = 0;
	
	// Sample all sound tracks currently playing.
	for (uint i = 0; i<audio->soundCount; i++) {
		if (audio->sounds[i].state == Running) {
			sum += playSong(&audio->sounds[i], time);
		}
	}
	sum >>= 16; // Convert from fixed point to unsigned int.
	
	// Set the DAC sample registers.
	*DAC0_CH0DATA = sum;
	*DAC0_CH1DATA = sum;
}

void restart(soundPlayer_t* player) {
	player->state = Running;
	player->noteIndex = 0;
	player->noteCounter = 0;
}