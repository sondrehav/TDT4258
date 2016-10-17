#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#include "ex2.h"
#include "sound_generator.h"
#include "sound_player.h"

/* Declaration of peripheral setup functions */
void setupGPIO();
void setupTimer(uint32_t period);
void setupDAC();
void setupNVIC();

/* Extern function found in songs.c */
extern void audioSetup();

/* Your code will start executing here */
int main(void)
{
	/* Call the peripheral setup functions */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);

	/* Enable interrupt handling */
	setupNVIC();

	audioSetup();

	*SCR = 6;

	__asm("wfi");
	while (1) ;

	return 0;
}

void setupNVIC()
{
	*ISER0 = 0x802;
}
