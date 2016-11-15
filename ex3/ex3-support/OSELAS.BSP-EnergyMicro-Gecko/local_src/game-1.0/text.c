#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

typedef uint16_t color;

static const uint32_t blockW = 5;
static const uint32_t blockH = 5;

static const uint16_t letters[25] = {
	63, 36, 52, 32, 56,
	63,  8, 16,  0, 56,
	63, 36, 56, 32, 24,
	61,  0, 16,  8, 56,
	47, 20,  4, 16, 40
};

void drawLetter(uint32_t x, uint32_t y, color numColor, uint16_t num, FILE* framebuffer)
{
	
	uint16_t shiftAmount;
	if (num <= 5) shiftAmount = num;
	else shiftAmount = 5;
	
	color colorArray[5*blockW];
	for (uint32_t i = 0; i < 5; i++) {
		for (uint32_t k = 0; k < blockH; k++) {
			fseek(framebuffer, sizeof(color) * (320 * (y+i*blockH+k) + x), SEEK_SET);
			for (uint32_t j = 0; j < 5; j++) {
				color tmpColor = numColor * ((letters[i*5+j] >> shiftAmount) & 1);
				for (uint32_t l = 0; l < blockW; l++) {
					colorArray[j*blockW+l] = tmpColor;
				}
			}
			fwrite(colorArray, sizeof(color), 5*blockW, framebuffer);
		}
	}
}

void drawWinner(uint32_t x, uint32_t y, color numColor, FILE* framebuffer)
{
	drawLetter(x, y, color, 4, framebuffer);
	drawLetter(x+6*blockW, y, color, 0, framebuffer);
	drawLetter(x+8*blockW, y, color, 3, framebuffer);
	drawLetter(x+14*blockW, y, color, 3, framebuffer);
	drawLetter(x+20*blockW, y, color, 2, framebuffer);
	drawLetter(x+24*blockW, y, color, 5, framebuffer);
	drawLetter(x+29*blockW, y, color, 1, framebuffer);
}