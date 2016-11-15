#include <stdbool.h>
#include "pong.h"
#include <time.h>

FILE* framebuffer;

typedef uint16_t color;

#define PIXEL_DIM 		5
#define V_SCREEN_WIDTH 	64
#define V_SCREEN_HEIGHT 48
#define SCREEN_WIDTH 	320
#define SCREEN_HEIGHT 	240

typedef struct PlayerState
{
	bool movingUp;
	bool movingDown;
	bool leftBoardPosition;
	uint32_t verticalPosition;
} PlayerState;

void drawBoard();
void enterGameLoop();
void drawNumber(uint32_t x, uint32_t y, color numColor, uint16_t num, FILE* framebuffer);

PlayerState leftPlayer;
PlayerState rightPlayer;

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
		fseek(framebuffer, sizeof(color) * (SCREEN_WIDTH * y + x0), SEEK_SET);
		for(uint32_t x = x0; x < x1; x++) {
			colorArray[x - x0] = col;
		}
		fwrite(colorArray, sizeof(color), x1 - x0, framebuffer);
	}
	free(colorArray);

}

void enterGame(FILE* framebufferDriver) {
	framebuffer = framebufferDriver;
	drawRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT, createColor(0,0,0));
	drawBoard();
	drawNumber(40,40, 0xffff, 2, framebuffer);
	enterGameLoop();
}

void onKeyDown(uint32_t key) {
	printf("%d\n",key);
	if(key == 1) leftPlayer.movingUp = true;
	else if(key == 3) leftPlayer.movingDown = true;
	else if(key == 5) rightPlayer.movingUp = true;
	else if(key == 7) rightPlayer.movingDown = true;
}

void onKeyUp(uint32_t key) {
	if(key == 1) leftPlayer.movingUp = false;
	else if(key == 3) leftPlayer.movingDown = false;
	else if(key == 5) rightPlayer.movingUp = false;
	else if(key == 7) rightPlayer.movingDown = false;
}

void toScreenSpace(uint32_t* x0, uint32_t* y0, uint32_t* x1, uint32_t* y1){
	*(x0) = *(x0) * SCREEN_WIDTH / V_SCREEN_WIDTH;
	*(x1) = *(x1) * SCREEN_WIDTH / V_SCREEN_WIDTH;
	*(y0) = *(y0) * SCREEN_HEIGHT / V_SCREEN_HEIGHT;
	*(y1) = *(y1) * SCREEN_HEIGHT / V_SCREEN_HEIGHT;
}

void drawBoard(){
	color color = createColor(255, 255, 255);
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

#define PLAYER_HEIGHT 			8
#define PLAYER_SCREEN_OFFSET	4

void playerMovement(PlayerState *player) {
	if(player->movingUp && 
		player->verticalPosition > (1 + PLAYER_HEIGHT / 2)) {
		player->verticalPosition -= 1;
	}
	if(player->movingDown && 
		player->verticalPosition < (V_SCREEN_HEIGHT -1 - PLAYER_HEIGHT / 2)) {
		player->verticalPosition += 1;
	}
}

void drawPlayer(PlayerState player, color colorIn) {
	uint32_t x0;
	if(player.leftBoardPosition) x0 = PLAYER_SCREEN_OFFSET;
	else x0 = V_SCREEN_WIDTH - PLAYER_SCREEN_OFFSET - 1;
	uint32_t x1 = x0 + 1;
	uint32_t y0 = player.verticalPosition - PLAYER_HEIGHT / 2;
	uint32_t y1 = player.verticalPosition + PLAYER_HEIGHT / 2;
	toScreenSpace(&x0, &y0, &x1, &y1);
	drawRectangle(x0, y0, x1, y1, colorIn);
}

void enterGameLoop() {

	color white = createColor(255, 255, 255);
	color black = createColor(0,0,0);
	leftPlayer.leftBoardPosition = true;
	leftPlayer.verticalPosition = V_SCREEN_HEIGHT / 2;
	rightPlayer.leftBoardPosition = false;
	rightPlayer.verticalPosition = V_SCREEN_HEIGHT / 2;

	struct timespec start;
	struct timespec now;

	uint32_t frameTimeNanoSec = 50000000;

	while(true) {

		clock_gettime(CLOCK_MONOTONIC, &start);

		drawPlayer(leftPlayer, black);
		drawPlayer(rightPlayer, black);
		playerMovement(&leftPlayer);
		playerMovement(&rightPlayer);

		// gamelogic goes here

		drawPlayer(leftPlayer, white);
		drawPlayer(rightPlayer, white);
		
		clock_gettime(CLOCK_MONOTONIC, &now);
		
		time_t secDiff = now.tv_sec - start.tv_sec;
		
		long remainingTime = frameTimeNanoSec - now.tv_sec + start.tv_sec - (secDiff * 1000000000);
		if (remainingTime < 0) remainingTime =0;
		now.tv_sec = 0;
		now.tv_nsec = remainingTime;

		while(nanosleep(&now, &now) < 0);

	}
}
