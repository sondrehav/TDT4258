#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

typedef uint16_t color;

static const blockW = 5;
static const blockH = 5;

static const uint16_t numbers[15] = {
	1021, 2031, 1021,
	 883, 1026,  925,
	 893, 2046,  893,
	 453,    2,  889,
	1007, 1903,  863
};

void drawNumber(uint32_t x, uint32_t y, color numColor, uint16_t num, FILE* framebuffer)
{
	uint16_t shiftAmount;
	if (num <= 10) shiftAmount = num;
	else shiftAmount = 10;
	
	color colorArray[3*blockW];
	for (uint32_t i = 0; i < 5; i++) {
		for (uint32_t k = 0; k < blockH; k++) {
			fseek(framebuffer, sizeof(color) * (320 * (y+i*blockH+k) + x), SEEK_SET);
			for (uint32_t j = 0; j < 3; j++) {
				Color tmpColor = numColor * ((numbers[i*3+j] << shiftAmount) & 1);
				for (uint32_t l = 0; l < blockW; l++) {
					colorArray[j*blockW+l] = tmpColor;
				}
			}
			fwrite(colorArray, sizeof(color), 3*blockW, framebuffer);
		}
	}
}