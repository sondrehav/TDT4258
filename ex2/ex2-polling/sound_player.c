#include "sound_player.h"

void initSoundPlayer(SoundPlayer* player, Song* song, SoundType soundType) {
	player->song = song;
	player->state = running;
	player->noteIndex = 0;
	player->noteCounter = 0;
	player->notePeriod = sampleRate * 60 / song->tempo;
	player->soundType = soundType;
}

fp playSong(SoundPlayer* player, uint time) {
	fp freq = getFreqNote(player->song->song[player->noteIndex]);
	fp sample;
	switch(player->soundType) {
		case SoundType.Saw:
			sample = sawWave(freq, time);
			break;
		case SoundType.Triangle:
			sample = triangleWave(freq, time);
			break;
		case SoundType.Square:
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
				player->state = PlayState.Done;
			}
		}
	}
	
	return sample;
}

void playAudio(Audio* audio, uint time) {
	uint sum = 0;
	for (uint i = 0; i<audio->soundCount; i++) {
		if (audio->sounds[i]->state == PlayState.running) {
			sum += playSong(audio->sounds[i], time);
		}
	}
	sum *= audio->volume;
	sum >>= 16;
	*DAC0_CH0DATA = sum;
	*DAC0_CH1DATA = sum;
}