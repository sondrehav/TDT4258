#include "sound_player.h"
#include "sound_generator.h"
#include "ex2.h"

void initSoundPlayer(soundPlayer_t* player, song_t* song, soundType_t soundType) {
	player->song = song;
	player->state = Running;
	player->noteIndex = 0;
	player->noteCounter = 0;
	player->notePeriod = SAMPLE_RATE * 60 / song->tempo;
	player->soundType = soundType;
}

fp playSong(soundPlayer_t* player, uint time) {
	fp freq = getFreqNote(player->song->song[player->noteIndex]);
	fp sample;
	switch(player->soundType) {
		case Saw:
			sample = sawWave(freq, time);
			break;
		case Triangle:
			sample = triangleWave(freq, time);
			break;
		case Square:
			sample = squareWave(freq, time);
			break;
	}
	
	player->noteCounter++;
	if (player->noteCounter == player->notePeriod) {
		player->noteIndex++;
		player->noteCounter = 0;
		if (player->noteIndex == player->song->length) {
			player->noteIndex = 0;
			if (!player->song->looping) {
				player->state = Done;
			}
		}
	}
	
	return sample;
}

void playAudio(audio_t* audio, uint time) {
	fp sum = 0;
	for (uint i = 0; i<audio->soundCount; i++) {
		if (audio->sounds[i].state == Running) {
			sum += playSong(&audio->sounds[i], time);
		}
	}
	sum *= audio->volume;
	sum >>= 16;
	*DAC0_CH0DATA = sum;
	*DAC0_CH1DATA = sum;
}