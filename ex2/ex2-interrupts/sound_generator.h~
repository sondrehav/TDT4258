#ifndef SOUND_GENERATOR_H
#define SOUND_GENERATOR_H

#include "ex2.h"

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
fp getFreqNote(uint note);

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

#endif
