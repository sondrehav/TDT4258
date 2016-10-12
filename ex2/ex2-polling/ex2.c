#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* The period between sound samples, in clock cycles */
#define	SAMPLE_PERIOD	292 // 14 MHz / 292 = 48 KHz

typedef uint32_t uint;
typedef uint32_t fp;

typedef struct Song
{
	uint* song;
	uint length;
	uint tempo;
	bool looping;
} Song_t;

static const uint sample_rate = 14000000 / SAMPLE_PERIOD;
static const uint volume = 128;

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
	Function for dividing one fixed point number by another,
	and returning a fixed point number.
	It uses floating point numbers internally.
*/
fp fixed_point_division(fp a, fp b);

/*
	Returns frequency of note in specified octave, tuned to A4 = 440Hz.
	Takes normal numbers, and return fixed point number.
	Note should be a value from 0-11 with the following value:
	C = 0, c# = 1, D = 2, D# = 3, E = 4, F = 5, F# = 6, G = 7, G# = 8, A = 9, A# = 10, B = 11
*/
fp getFrequency(uint note, uint octave);
fp getFreqNote(uint note);

/*
	Function called when new data should be pushed to the DAC.
	Function is called at the sample rate, 48 KHz.
	time is the time sinze the start in sample rate ticks.
*/
void pushDataToDAC(uint time);
void playBeat(uint beat_index, uint time);

/*
	Functions for generating waveforms.
	They take a frequency in fixed point format,
	and a time in sample rate ticks,
	and return a value between 0 and 1,
	in fixed point format.
*/
fp sawWave(fp frequency, uint time);
fp squareWave(fp frequency, uint time);
fp triangleWave(fp frequency, uint time);

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
	uint value = (sum*volume) >> 16;
	*DAC0_CH0DATA = value;
	*DAC0_CH1DATA = value;	
}



fp sawWave(fp frequency, uint time)
{
	uint period = (sample_rate << 16) / frequency; // Find wave period.
	fp value = (time % period) << 16; // Value is in range 0 to period.
	value /= period; // Value is in range 0 to 1.
	return value;
}

fp squareWave(fp frequency, uint time)
{
	uint period = (sample_rate << 16) / frequency; // Find wave period.
	uint time_point = (time % period); // Find time point in period.
	if (time_point < period / 2) return 0;
	else return 1<<16;
}

fp triangleWave(fp frequency, uint time)
{
	uint period = (sample_rate << 16) / frequency; // Find wave period.
	uint time_point = (time % period); // Find time point in period.
	if (time_point < period / 2) return time_point<<17 / period;
	else return (period-time_point)<<17 / period;
}



fp getFreqNote(uint note)
{
	return getFrequency(note%12, note/12);
}

fp getFrequency(uint note, uint octave)
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

fp fixed_point_division(fp a, fp b)
{
	float res = (float)a / (float)b;
	return (fp)(res*(1<<16));
}