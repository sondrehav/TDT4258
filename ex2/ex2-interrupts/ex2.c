#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#include "ex2.h"
#include "sound_generator.h"
#include "sound_player.h"

/* 
  TODO calculate the appropriate sample period for the sound wave(s) 
  you want to generate. The core clock (which the timer clock is derived
  from) runs at 14 MHz by default. Also remember that the timer counter
  registers are 16 bits.
*/
/* The period between sound samples, in clock cycles */

/* Declaration of peripheral setup functions */
void setupGPIO();
void setupTimer(uint32_t period);
void setupDAC();
void setupNVIC();



/* Sound data and variables. */
uint songData[] = {
	48, 50, 52, 53, 55, 55, 55, 55, 57, 57, 57, 57, 
	55, 55, 55, 55, 53, 53, 53, 53, 52, 52, 52, 52, 
	50, 50, 50, 50, 48, 48, 48, 48
};

uint songData2[] = {
	48, 52, 55, 60, 64, 55, 60, 64,
	48, 52, 55, 60, 64, 55, 60, 64,
	48, 50, 55, 62, 65, 55, 62, 65, 
	48, 50, 55, 62, 65, 55, 62, 65, 
	47, 50, 55, 61, 65, 55, 61, 65, 
	47, 50, 55, 61, 65, 55, 61, 65, 
	48, 52, 55, 60, 64, 55, 60, 64,
	48, 52, 55, 60, 64, 55, 60, 64
};

uint song1up[] = {
   64, 67, 76, 72, 74, 79
};

uint songData4[] = {
	100, 90, 80, 70, 60, 50, 40, 30, 20, 10, 0
};



song_t songs[4];
soundPlayer_t soundPlayers[4];
audio_t audio = {soundPlayers, 4};

song_t song1 = {songData, 32, 240, 0};
song_t song2 = {songData2, 64, 480, 0};
song_t song3 = {song1up, 6, 360, 0};
song_t song4 = {songData4, 11, 120, 0};

uint time;

/* Your code will start executing here */
int main(void)
{
	/* Call the peripheral setup functions */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);

	/* Enable interrupt handling */
	setupNVIC();

	/* Audio setup. */
	songs[0] = song1;
	songs[1] = song2;
	songs[2] = song3;
	songs[3] = song4;
	
	initSoundPlayer(soundPlayers, songs, Saw, 4);
	initSoundPlayer(soundPlayers+1, songs+1, Saw, 4);
	initSoundPlayer(soundPlayers+2, songs+2, Square, 4);
	initSoundPlayer(soundPlayers+3, songs+3, Saw, 4);
	
	soundPlayers[0].state = Paused;
	soundPlayers[1].state = Paused;
	soundPlayers[2].state = Paused;
   soundPlayers[3].state = Paused;
	
	time = 0;
	
	/* TODO for higher energy efficiency, sleep while waiting for interrupts
	   instead of infinite loop for busy-waiting
	 */
	while (1) ;

	return 0;
}

void setupNVIC()
{
	/* TODO use the NVIC ISERx registers to enable handling of interrupt(s)
	   remember two things are necessary for interrupt handling:
	   - the peripheral must generate an interrupt signal
	   - the NVIC must be configured to make the CPU handle the signal
	   You will need TIMER1, GPIO odd and GPIO even interrupt handling for this
	   assignment.
	 */
	*ISER0 = 0x802;
}

/* if other interrupt handlers are needed, use the following names: 
   NMI_Handler
   HardFault_Handler
   MemManage_Handler
   BusFault_Handler
   UsageFault_Handler
   Reserved7_Handler
   Reserved8_Handler
   Reserved9_Handler
   Reserved10_Handler
   SVC_Handler
   DebugMon_Handler
   Reserved13_Handler
   PendSV_Handler
   SysTick_Handler
   DMA_IRQHandler
   GPIO_EVEN_IRQHandler
   TIMER0_IRQHandler
   USART0_RX_IRQHandler
   USART0_TX_IRQHandler
   USB_IRQHandler
   ACMP0_IRQHandler
   ADC0_IRQHandler
   DAC0_IRQHandler
   I2C0_IRQHandler
   I2C1_IRQHandler
   GPIO_ODD_IRQHandler
   TIMER1_IRQHandler
   TIMER2_IRQHandler
   TIMER3_IRQHandler
   USART1_RX_IRQHandler
   USART1_TX_IRQHandler
   LESENSE_IRQHandler
   USART2_RX_IRQHandler
   USART2_TX_IRQHandler
   UART0_RX_IRQHandler
   UART0_TX_IRQHandler
   UART1_RX_IRQHandler
   UART1_TX_IRQHandler
   LEUART0_IRQHandler
   LEUART1_IRQHandler
   LETIMER0_IRQHandler
   PCNT0_IRQHandler
   PCNT1_IRQHandler
   PCNT2_IRQHandler
   RTC_IRQHandler
   BURTC_IRQHandler
   CMU_IRQHandler
   VCMP_IRQHandler
   LCD_IRQHandler
   MSC_IRQHandler
   AES_IRQHandler
   EBI_IRQHandler
   EMU_IRQHandler
*/
