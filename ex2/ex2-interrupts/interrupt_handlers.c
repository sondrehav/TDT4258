#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#include "ex2.h"
#include "sound_generator.h"
#include "sound_player.h"

/* TIMER1 interrupt handler */

	
void playBeat(uint beat_index, uint time);


uint songData[] = {
	48, 50, 52, 53, 55, 55, 55, 55, 57, 57, 57, 57, 
	55, 55, 55, 55, 53, 53, 53, 53, 52, 52, 52, 52, 
	50, 50, 50, 50, 48, 48, 48, 48
};
Song_t song = {songData, 32, 120, 1};

uint32_t beat_period_counter = 0;
uint32_t count = 0;
uint32_t beat_index = 0;


void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/*
	   TODO feed new samples to the DAC
	   remember to clear the pending interrupt by writing 1 to TIMER1_IFC
	 */

	uint beat_period = SAMPLE_RATE * 60 / song.tempo;
	playBeat(beat_index, count);
			
	beat_period_counter++;
	if (beat_period_counter == beat_period) {
		beat_period_counter = 0;
		beat_index++;
		if (beat_index == song.length) beat_index = 0;
	}
	
	count++;

	*TIMER1_IFC = 0x1;
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	/* TODO handle button pressed event, remember to clear pending interrupt */
	*GPIO_PA_DOUT = 0xffffffff ^ *GPIO_PA_DOUT;
	*GPIO_IFC = *GPIO_IF;
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	/* TODO handle button pressed event, remember to clear pending interrupt */
	*GPIO_PA_DOUT = 0xffffffff ^ *GPIO_PA_DOUT;
	*GPIO_IFC = *GPIO_IF;
}

void playBeat(uint beat_index, uint time)
{
	fp frequency = getFreqNote(song.song[beat_index]);
	fp sum = sawWave(frequency, time);
	uint value = (sum*VOLUME) >> 16;
	*DAC0_CH0DATA = value;
	*DAC0_CH1DATA = value;	
}