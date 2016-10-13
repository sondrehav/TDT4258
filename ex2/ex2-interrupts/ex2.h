#ifndef EX2_H
#define EX2_H

#include <stdint.h>
#include <stdbool.h>

/* The period between sound samples, in clock cycles */
#define SAMPLE_RATE		12000
#define	SAMPLE_PERIOD	14000000 / SAMPLE_PERIOD // 14 MHz / 292 = 48 KHz
#define VOLUME 			128

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