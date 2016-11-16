#include "sound_generator.h"

fp sawWave(fp frequency, uint time)
{
	uint period = (SAMPLE_RATE << 16) / frequency;	// Find wave period.
	fp value = (time % period) << 16;	// Value is in range 0 to period.
	value /= period;	// Value is in range 0 to 1.
	return value;
}

fp squareWave(fp frequency, uint time)
{
	uint period = (SAMPLE_RATE << 16) / frequency;	// Find wave period.
	uint time_point = (time % period);	// Find time point in period.
	return (time_point < period / 2) << 16;
}

fp triangleWave(fp frequency, uint time)
{
	uint period = (SAMPLE_RATE << 16) / frequency;	// Find wave period.
	uint time_point = (time % period);	// Find time point in period.
	uint time_point_2 = period - time_point;
	uint k = (time_point < period / 2);
	return (((time_point * k) + (time_point_2 * k ^ 1)) << 17) / period;
}

fp getFreqNote(uint note)
{
	return getFrequency(note % 12, note / 12);
}

fp getFrequency(uint note, uint octave)
{
	// 440 * 2^((octave*12 + note - 57)/12) =
	// 440 / 2^(57/12) * 2^octave * 2^(note/12)

	fp res = 1071618;	// 440 / 2^(57/12) in fixed point form.
	res <<= octave;		// Multiply with 2^octave.

	// Table of 2^(note/12) for note=0 to note=11, in fixed point form.
	static const fp note_const[] = { 65536, 69433, 73562, 77936,
		82570, 87480, 92682, 98193, 104032, 110218, 116772, 123715
	};

	return fixed_point_mul(res, note_const[note]);	// Multiply with 2^(note/12).
}
