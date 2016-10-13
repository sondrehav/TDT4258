enum PlayState 
{
	Running, Ready, Done, Paused
};

typedef struct SoundPlayer 
{
	Song* song;
	PlayState state;
	uint noteIndex;
	uint volume;
	SoundType soundType;
};

void playSong(SoundPlayer* player, uint time) {
	switch(player->state) {
		case State.Running:
			fp freq = getFreqNote(player->song->song[player->noteIndex]);
			fp sample;
			switch(player->soundType) {
				case SoundType.Saw:
					sample = sawWave(freq, player->time);
					break;
				case SoundType.Triangle:
					sample = triangleWave(freq, player->time);
					break;
				case SoundType.Square:
					sample = squareWave(freq, player->time);
					break;
			}
			uint value = (sample * player->volume) >> 16;
			*DAC0_CH0DATA = value;
			*DAC0_CH1DATA = value;

	}
}