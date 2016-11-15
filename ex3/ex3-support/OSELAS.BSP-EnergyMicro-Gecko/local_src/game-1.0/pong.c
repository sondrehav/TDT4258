
#include "pong.h"

FILE* framebuffer;

typedef uint16_t color;

#define PIXEL_DIM 		5
#define V_SCREEN_WIDTH 	64
#define V_SCREEN_HEIGHT 48
#define SCREEN_WIDTH 	320
#define SCREEN_HEIGHT 	240

void drawBoard();

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
	drawRectangle(0,0,320,240, createColor(0,0,0));
	drawBoard();
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

void toScreenSpace(uint32_t* x0, uint32_t* y0, uint32_t* x1, uint32_t* y1){
	*(x0) = *(x0) * SCREEN_WIDTH / V_SCREEN_WIDTH;
	*(x1) = *(x1) * SCREEN_WIDTH / V_SCREEN_WIDTH;
	*(y0) = *(y0) * SCREEN_HEIGHT / V_SCREEN_HEIGHT;
	*(y1) = *(y1) * SCREEN_HEIGHT / V_SCREEN_HEIGHT;
}

void drawBoard(){
	color = createColor(255, 255, 255);
	{ // mid line
		uint32_t mid = V_SCREEN_WIDTH / 2;
		uint32_t x0 = mid;
		uint32_t x1 = mid + 1;
		uint32_t y0 = 0;
		uint32_t y1 = V_SCREEN_HEIGHT;
		toScreenSpace(&x0, &y0, &x1, &y1);
		drawRectangle(x0, y0, x1, y1, color);
	}
	{ // top line
		uint32_t x0 = 0;
		uint32_t x1 = V_SCREEN_WIDTH;
		uint32_t y0 = 0;
		uint32_t y1 = 1;
		toScreenSpace(&x0, &y0, &x1, &y1);
		drawRectangle(x0, y0, x1, y1, color);
	}
	{ // bottom line
		uint32_t x0 = 0;
		uint32_t x1 = V_SCREEN_WIDTH;
		uint32_t y0 = V_SCREEN_HEIGHT - 1;
		uint32_t y1 = V_SCREEN_HEIGHT;
		toScreenSpace(&x0, &y0, &x1, &y1);
		drawRectangle(x0, y0, x1, y1, color);
	}
	{ // left line
		uint32_t x0 = 0;
		uint32_t x1 = 1;
		uint32_t y0 = 0;
		uint32_t y1 = V_SCREEN_HEIGHT;
		toScreenSpace(&x0, &y0, &x1, &y1);
		drawRectangle(x0, y0, x1, y1, color);
	}
	{ // right line
		uint32_t x0 = V_SCREEN_WIDTH - 1;
		uint32_t x1 = V_SCREEN_WIDTH;
		uint32_t y0 = 0;
		uint32_t y1 = V_SCREEN_HEIGHT;
		toScreenSpace(&x0, &y0, &x1, &y1);
		drawRectangle(x0, y0, x1, y1, color);
	}
}