#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* The period between sound samples, in clock cycles */
#define   SAMPLE_PERIOD   292 // 14 MHz / 292 = 48 KHz

typedef uint32_t uint;
typedef uint32_t fp;

static const sample_rate = 14000000 / SAMPLE_PERIOD;

/* Declaration of peripheral setup functions */
void setupGPIO();
void setupTimer(uint32_t period);
void setupDAC();

/*
   This implementation uses a fixed point number format.
   It stores positive numbers in a uint32_t, with 16 bit before and after the radix point.
   A whole positive number can be converted to the fixed point format,
   by bit shifting left, or multiply by 2^16.
   And back, by bit shifting right, or divide by 2^16.
   The format does not store negative numbers.
   This is done to avoid having to use slow floating point numbers.
   It is specified when a function takes, or returns, this format.
*/

/*
   Declaration of fixed point multiplication, implemented in fixed_point_mul.s
   Used to multiply two fixed point numbers.
   Also returns a fixed point number.
   For multiplication between a fixed point and a normal number,
   normal multiplication can be used.
*/
extern fp fixed_point_mul(fp a, fp b);

/*
   Returns frequency of note in specified octave, tuned to A4 = 440Hz.
   Takes normal numbers, and return fixed point number.
   Note should be a value from 0-11 with the following value:
   C = 0, c# = 1, D = 2, D# = 3, E = 4, F = 5, F# = 6, G = 7, G# = 8, A = 9, A# = 10, B = 11
*/
fp getFrequency(uint note, uint octave);

/*
   Function called when new data should be pushed to the DAC.
   Function is called at the sample rate, 48 KHz.
   time is the time sinze the start in sample rate tick.
*/
void pushDataToDAC(uint time);

int main(void)
{
	/* Call the peripheral setup functions */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);
	
	
	
	// Polling loop
	uint count = 0; 
	uint lastTimerValue = 0;
	while (1) {
		uint timerValue = *TIMER1_CNT;
		
		// Check if new data should be pushed to the DAC.
		if(timerValue <= 150 && lastTimerValue > 150){
			pushDataToDAC(count);
			
			if (count == 4294967295) count = 0;
			else count++;
		}
		lastTimerValue = timerValue;
	}

	return 0;
}

void pushDataToDAC(uint count)
{
	*DAC0_CH0DATA = count%109;
	*DAC0_CH1DATA = count%109;
}

fp getFrequency(uint note, uint octave);
{
	// 440 * 2^((octave*12 + note - 57)/12) =
	// 440 / 2^(57/12) * 2^octave * 2^(note/12)
	
	fp res = 1071618; // 440 / 2^(57/12) in fixed point form.
	res <<= octave; // Multiply with 2^octave.
	
	// Table of 2^(note/12) for note=0 to note=11, in fixed point form.
	static const fp note_const[] = {65536, 69433, 73562, 77936,
	82570, 87480, 92682, 98193, 104032, 110218, 116772, 123715};
	
	return fixed_point_mul(res, note_const[note]); // Multiply with 2^(note/12).
}