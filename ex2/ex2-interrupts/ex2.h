#ifndef EX2_H
#define EX2_H

#include <stdint.h>
#include <stdbool.h>

/* The period between sound samples, in clock cycles */
#define	SAMPLE_PERIOD	292 // 14 MHz / 292 = 48 KHz
#define VOLUME 			128
#define SAMPLE_RATE		14000000 / SAMPLE_PERIOD

typedef uint32_t uint;
typedef uint32_t fp;

typedef struct Song
{
	uint* song;
	uint length;
	uint tempo;
	bool looping;
} Song_t;

#endif