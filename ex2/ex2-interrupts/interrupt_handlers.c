#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#include "ex2.h"
#include "sound_generator.h"
#include "sound_player.h"

void selectSong();

extern audio_t audio;
extern uint time;

/* TIMER1 interrupt handler */

void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/*
	   TODO feed new samples to the DAC
	   remember to clear the pending interrupt by writing 1 to TIMER1_IFC
	 */
	playAudio(&audio, time);
	time++;

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
	if (((~*GPIO_PC_DIN) & 0x1) == 0x1)
	{
		audio.sounds[0].state = Running;
		*GPIO_PA_DOUT = 0x0fff ^ *GPIO_PA_DOUT;
	}
	else if (((~*GPIO_PC_DIN) & 0x2) == 0x2)
	{
		audio.sounds[1].state = Running;
		*GPIO_PA_DOUT = 0xf0ff ^ *GPIO_PA_DOUT;
	}
	else if (((~*GPIO_PC_DIN) & 0x4) == 0x4)
	{
		audio.sounds[2].state = Running;
		*GPIO_PA_DOUT = 0x5fff ^ *GPIO_PA_DOUT;
	}
	else if (((~*GPIO_PC_DIN) & 0x8) == 0x8)
	{
		audio.sounds[3].state = Running;
		*GPIO_PA_DOUT = 0xf5ff ^ *GPIO_PA_DOUT;
	}
}