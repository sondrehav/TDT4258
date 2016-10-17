#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#include "ex2.h"
#include "sound_generator.h"
#include "sound_player.h"

/* Select what song to play based on what button is pressed. */
void selectSong();

/* Extern variables found in songs.c */
extern audio_t audio;
extern uint time;
extern uint runningCount;
extern bool noAudio;

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	playAudio(&audio, time);
	time++;
	
	*SCR = 6*noAudio; // Sleep if no audio.
	
	*TIMER1_IFC = 0x1;
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	*GPIO_IFC = *GPIO_IF;
	selectSong();
	noAudio = 0;
	*SCR = 0;
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	*GPIO_IFC = *GPIO_IF;
	selectSong();
	noAudio = 0;
	*SCR = 0;
}

void selectSong(){
	if (((~*GPIO_PC_DIN) & 0x1) == 0x1)
	{
		runningCount += (audio.sounds[0].state != Running);
		audio.sounds[0].state = Running;
	}
	else if (((~*GPIO_PC_DIN) & 0x2) == 0x2)
	{
		runningCount += (audio.sounds[1].state != Running);
		audio.sounds[1].state = Running;
	}
	else if (((~*GPIO_PC_DIN) & 0x4) == 0x4)
	{
		runningCount += (audio.sounds[2].state != Running);
		audio.sounds[2].state = Running;
	}
	else if (((~*GPIO_PC_DIN) & 0x8) == 0x8)
	{
		runningCount += (audio.sounds[3].state != Running);
		audio.sounds[3].state = Running;
	}
}