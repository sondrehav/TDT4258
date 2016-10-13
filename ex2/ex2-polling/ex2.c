#include <stdint.h>
#include <stdbool.h>

#include "sound_generator.h"

#include "efm32gg.h"

#include "ex2.h"



/* Declaration of peripheral setup functions */
void setupGPIO();
void setupTimer(uint32_t period);
void setupDAC();

/*
	Function called when new data should be pushed to the DAC.
	Function is called at the sample rate, 48 KHz.
	time is the time sinze the start in sample rate ticks.
*/
void playBeat(uint beat_index, uint time);


uint songData[] = {
	48, 50, 52, 53, 55, 55, 55, 55, 57, 57, 57, 57, 
	55, 55, 55, 55, 53, 53, 53, 53, 52, 52, 52, 52, 
	50, 50, 50, 50, 48, 48, 48, 48
};
Song_t song = {songData, 32, 120, 1};

int main(void)
{
	/* Call the peripheral setup functions */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);
	
	// Polling loop
	uint count = 0; 
	uint lastTimerValue = 0;
	
	uint beat_period = sample_rate * 60 / song.tempo;
	uint beat_period_counter = 0;
	uint beat_index = 0;
	
	while (1) {
		uint timerValue = *TIMER1_CNT;
		
		// Check if new data should be pushed to the DAC.
		if (timerValue <= 150 && lastTimerValue > 150) {
			playBeat(beat_index, count);
			
			beat_period_counter++;
			if (beat_period_counter == beat_period) {
				beat_period_counter = 0;
				beat_index++;
				if (beat_index == song.length) beat_index = 0;
			}
			
			if (count == 0xffffffff) count = 0;
			else count++;
		}
		lastTimerValue = timerValue;
	}

	return 0;
}

void playBeat(uint beat_index, uint time)
{
	fp fq = getFreqNote(song.song[beat_index]);
	fp sum = sawWave(fq, time);
	uint value = (sum*VOLUME) >> 16;
	*DAC0_CH0DATA = value;
	*DAC0_CH1DATA = value;	
}

