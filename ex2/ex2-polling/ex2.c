#include <stdint.h>
#include <stdbool.h>

#include "math.h"
#include "efm32gg.h"

#include "math.h"

/* 
  TODO calculate the appropriate sample period for the sound wave(s) 
  you want to generate. The core clock (which the timer clock is derived
  from) runs at 14 MHz by default. Also remember that the timer counter
  registers are 16 bits.
*/
/* The period between sound samples, in clock cycles */
#define   SAMPLE_PERIOD   292
typedef uint32_t uint;

/* Declaration of peripheral setup functions */
void setupGPIO();
void setupTimer(uint period);
void setupDAC();
void setupNVIC();

float sawWave(float frequency, uint time);
float squareWave(float frequency, uint time);
float triangleWave(float frequency, uint time);

/* Your code will start executing here */
int main(void)
{
	/* Call the peripheral setup functions */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);

	/* Enable interrupt handling */
	setupNVIC();

	/* TODO for higher energy efficiency, sleep while waiting for interrupts
	   instead of infinite loop for busy-waiting
	 */

   //int notes[] = {0, 2, 4, 5, 7, 9, 11, 12};
   float notes[] = {110.0, 146.83, 261.63, 587.33, 880.0, 1318.51, 1760.0, 2349.32};
   int noteIndex = 0;

	uint count = 0; 
	uint lastTimerValue = 0;
	while (1) {
		uint timerValue = *TIMER1_CNT;
		if(timerValue <= 150 && lastTimerValue > 150){
		
		//440.0 * pow(2.0, (float) 12.0 / 12.0 )
        //float value = sawWave(notes[noteIndex], count);
		//float value = squareWave(440.0, count);
		float value = sawWave(440.0, count);
        value *= 128.0;

			*DAC0_CH0DATA = (uint) value;
			*DAC0_CH1DATA = (uint) value;
			
			count++;
         if((count % 24000) == 0) 
         {
            noteIndex++;
            if(noteIndex >= 8) {
               noteIndex = 0;
            }
         }
		}
		lastTimerValue = timerValue;
	}
	return 0;
}

float sawWave(float frequency, uint time)
{
   float d = (float) 48000 / frequency;
   return (float)(time % (uint)d) / d;
}

float squareWave(float frequency, uint time)
{
   float d = (float) 48000 / frequency;
   uint dt = time % (uint)d;
   if(dt < d/2) return 0.0;
   else return 1.0;
}

float triangleWave(float frequency, uint time)
{
   float d = (float) 48000 / frequency;
   uint dt = time % (uint)d;
   if(dt < d/2) return 2.0 * (float)dt / d;
   else return 2.0 * (float)(1-dt) / d;
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
