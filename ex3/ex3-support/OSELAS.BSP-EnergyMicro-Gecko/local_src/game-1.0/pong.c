
#include "pong.h"

FILE* framebuffer;

typedef uint16_t color;

static color createColor(uint8_t r, uint8_t g, uint8_t b)
{
	color c = 0;
	c |= (((uint16_t)r >> 3) << 11);
	c |= (((uint16_t)g >> 2) << 5);
	c |= (((uint16_t)b >> 3) << 0);
	return c;
}

static void drawRectangle(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, color col) {
	

	color* colorArray = (color*) malloc((x1 - x0) * sizeof(color));
	for(uint32_t y = y0; y < y1; y++) {
		fseek(framebuffer, sizeof(color) * (320 * y + x0), SEEK_SET);
		for(uint32_t x = x0; x < x1; x++) {
			colorArray[x - x0] = col;
		}
		fwrite(colorArray, sizeof(color), x1 - x0, framebuffer);
	}
	free(colorArray);

}

void enterGame(FILE* framebufferDriver) {
	framebuffer = framebufferDriver;
	for (int j=0; j<30; j++){
		sleep(1);
	}
}

void onKeyDown(uint32_t key) {
	drawRectangle(key * 20, key * 20, (key + 1) * 20, (key + 1) * 20, createColor(127, 127, 255));
}

void onKeyUp(uint32_t key) {
	drawRectangle(key * 20, key * 20, (key + 1) * 20, (key + 1) * 20, createColor(0, 0, 0));
}
