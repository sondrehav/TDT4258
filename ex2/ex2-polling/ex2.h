#ifndef EX2_H
#define EX2_H

#include <stdint.h>
#include <stdbool.h>

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

typedef struct musicPlayer
{
	Song* song;
	uint currentTime;
	
} musicPlayer_t;