#include "pong.h"

bool running = true;
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

void enterGame(FILE* framebufferDriver) {
	framebuffer = framebufferDriver;
	static uint16_t* arr = {7, 643, 1, 35635, 1244, 4574, 58753, 34325};
	fwrite(arr, sizeof(uint16_t), 8, framebuffer);
	sleep(30);
}

void onKeyDown(uint32_t key) {
	static uint16_t* arr = {5 * key, 11 * key, 13 * key, 17 * key, 19 * key, 23 * key, 29 * key, 31 * key};
	fwrite(arr, sizeof(uint16_t), 8, framebuffer);
}

void onKeyUp(uint32_t key) {
	static uint16_t* arr = {0, 0, 0, 0, 0, 0, 0, 0};
	fwrite(arr, sizeof(uint16_t), 8, framebuffer);	
}