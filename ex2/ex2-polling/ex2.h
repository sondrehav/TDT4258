#ifndef EX2_H
#define EX2_H

#include <stdint.h>
#include <stdbool.h>

/* The frequency at which new samples are given to the DAC. */
#define SAMPLE_RATE		12000
#define	SAMPLE_PERIOD	14000000 / SAMPLE_RATE

typedef uint32_t uint;
typedef uint32_t fp;

#endif