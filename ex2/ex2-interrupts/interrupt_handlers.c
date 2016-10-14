#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#include "ex2.h"
#include "sound_generator.h"
#include "sound_player.h"

/* TIMER1 interrupt handler */

	
void playBeat(uint beat_index, uint time);
void selectSong();

uint currentSong = 0;

uint songData[] = {
	48, 52, 55, 60, 64, 55, 60, 64,
	48, 52, 55, 60, 64, 55, 60, 64,
	48, 50, 55, 62, 65, 55, 62, 65, 
	48, 50, 55, 62, 65, 55, 62, 65, 
	47, 50, 55, 61, 65, 55, 61, 65, 
	47, 50, 55, 61, 65, 55, 61, 65, 
	48, 52, 55, 60, 64, 55, 60, 64,
	48, 52, 55, 60, 64, 55, 60, 64
};
Song_t song = {songData, 64, 120, 1};

uint songData2[] = {
	48 +5, 52 +5, 55+5, 60+5, 64+5, 60+5, 55+5, 52+5
};
Song_t song2 = {songData2, 8, 1920, 1};



uint32_t beat_period_counter = 0;
uint32_t count = 0;
uint32_t beat_index = 0;


void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/*
	   TODO feed new samples to the DAC
	   remember to clear the pending interrupt by writing 1 to TIMER1_IFC
	 */
	Song_t s = currentSong == 0 ? song : song2;
	uint beat_period = SAMPLE_RATE * 60 / s.tempo;
	playBeat(beat_index, count);
			
	beat_period_counter++;
	if (beat_period_counter >= beat_period) {
		beat_period_counter = 0;
		beat_index++;
		if (beat_index >= s.length) beat_index = 0;
	}
	
	count++;

	*TIMER1_IFC = 0x1;
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	/* TODO handle button pressed event, remember to clear pending interrupt */
	*GPIO_IFC = *GPIO_IF;
	selectSong();
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	/* TODO handle button pressed event, remember to clear pending interrupt */
	*GPIO_IFC = *GPIO_IF;
	selectSong();
}

void selectSong(){
	if (((~*GPIO_PC_DIN) & 0x1) == 0x1){
		currentSong = 0;
		*GPIO_PA_DOUT = 0x0fff ^ *GPIO_PA_DOUT;
	}
	else if (((~*GPIO_PC_DIN) & 0x2) == 0x2)
	{
		*GPIO_PA_DOUT = 0xf0ff ^ *GPIO_PA_DOUT;
		currentSong = 1;
	}
	else if (((~*GPIO_PC_DIN) & 0x4) == 0x4){
		*GPIO_PA_DOUT = 0x5fff ^ *GPIO_PA_DOUT;
		currentSong = 0;
	}
	else if (((~*GPIO_PC_DIN) & 0x8) == 0x8){
		*GPIO_PA_DOUT = 0xf5ff ^ *GPIO_PA_DOUT;
		currentSong = 1;
	}
}

void playBeat(uint beat_index, uint time)
{
	Song_t s = currentSong == 0 ? song : song2;
	fp frequency = getFreqNote(s.song[beat_index]);
	fp sum = sawWave(frequency, time);
	uint value = (sum*VOLUME) >> 16;
	*DAC0_CH0DATA = value;
	*DAC0_CH1DATA = value;	
}