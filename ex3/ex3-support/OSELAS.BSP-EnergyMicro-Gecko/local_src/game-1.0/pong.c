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

typedef struct Ball
{
	uint32_t x;
	uint32_t y;
	uint32_t lastX;
	uint32_t lastY;
} Ball;

void drawBoard();
void enterGameLoop();
void drawNumber(uint32_t x, uint32_t y, color numColor, uint16_t num, FILE* framebuffer);

Ball ball;
PlayerState leftPlayer;
PlayerState rightPlayer;

uint32_t playerLeftScore;
uint32_t playerRightScore;

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

void ballMovement() {
	uint32_t x = ball.x;
	uint32_t y = ball.y;
	ball.x = ball.x + ball.x - ball.lastX;
	ball.y = ball.y + ball.y - ball.lastY;
	ball.lastX = x;
	ball.lastY = y;
}

void ballPlayerTest(PlayerState* player) {
	uint32_t x_player;
	if(player->leftBoardPosition) x_player = PLAYER_SCREEN_OFFSET;
	else x_player = V_SCREEN_WIDTH - PLAYER_SCREEN_OFFSET - 1;
	if(ball.x == x_player) {
		uint32_t y0 = player->verticalPosition - PLAYER_HEIGHT / 2;
		uint32_t y1 = player->verticalPosition + PLAYER_HEIGHT / 2;
		if(ball.y >= y0 && ball.y < y1) {
			// hit
			if(player->leftBoardPosition) {
				ball.lastX = ball.lastX - 1;
			} else {
				ball.lastX = ball.lastX + 1;
			}
		}
	}
}

void redrawNumber(){
	int leftX = 27;
	int rightX = 34;
	int y = 7;
	toScreenSpace(&leftX, &rightX, &y, &y); // TODO: Ugly code is ugly
	color c = createColor(255, 255, 255);
	drawNumber(leftX, y, c, playerLeftScore, framebuffer);
	drawNumber(rightX, y, c, playerRightScore, framebuffer);
}

void resetScore(){
	playerRightScore = 0;
	playerLeftScore = 0;
}

void reset() {
	
	leftPlayer.leftBoardPosition = true;
	leftPlayer.verticalPosition = V_SCREEN_HEIGHT / 2;
	rightPlayer.leftBoardPosition = false;
	rightPlayer.verticalPosition = V_SCREEN_HEIGHT / 2;

	ball.x = V_SCREEN_WIDTH / 2;
	ball.y = V_SCREEN_HEIGHT / 2;
	ball.lastX = V_SCREEN_WIDTH / 2 - 1;
	ball.lastY = V_SCREEN_HEIGHT / 2 - 1;

	if(playerLeftScore > 9 || playerRightScore > 9) {
		resetScore();
	}

}

void gameLogic() {
	if(ball.y <= 0) {
		ball.y = 1;
		ball.lastY = 0;
	} else if(ball.y >= V_SCREEN_HEIGHT - 1) {
		ball.y = V_SCREEN_HEIGHT - 2;
		ball.lastY = V_SCREEN_HEIGHT - 1;
	}
	if(ball.x <= 0) {
		playerRightScore += 1;
		reset();
		redrawNumber();
	} else if(ball.x >= V_SCREEN_WIDTH - 1) {
		playerLeftScore += 1;
		reset();
		redrawNumber();
	}

}

void drawBall(color c) {
	if(ball.x == V_SCREEN_WIDTH / 2) return;
	uint32_t x0 = ball.x;
	uint32_t x1 = x0 + 1;
	uint32_t y0 = ball.y;
	uint32_t y1 = y0 + 1;
	toScreenSpace(&x0, &y0, &x1, &y1);
	drawRectangle(x0, y0, x1, y1, c);
}


void enterGameLoop() {

	color white = createColor(255, 255, 255);
	color black = createColor(0,0,0);
	
	playerLeftScore = 0;
	playerRightScore = 0;

	reset();
	redrawNumber();

	struct timespec start;
	struct timespec now;

	uint32_t frameTimeNanoSec = 50000000;

	while(true) {

		clock_gettime(CLOCK_MONOTONIC, &start);

		drawPlayer(leftPlayer, black);
		drawPlayer(rightPlayer, black);
		drawBall(black);

		playerMovement(&leftPlayer);
		playerMovement(&rightPlayer);
		ballMovement();

		ballPlayerTest(&leftPlayer);
		ballPlayerTest(&rightPlayer);

		gameLogic();

		drawPlayer(leftPlayer, white);
		drawPlayer(rightPlayer, white);
		drawBall(white);
		
		clock_gettime(CLOCK_MONOTONIC, &now);
		
		time_t secDiff = now.tv_sec - start.tv_sec;
		
		long remainingTime = frameTimeNanoSec - now.tv_sec + start.tv_sec - (secDiff * 1000000000);
		if (remainingTime < 0) {
			printf("Max frame time!\n");
			remainingTime = 0;
		}
		now.tv_sec = 0;
		now.tv_nsec = remainingTime;

		while(nanosleep(&now, &now) < 0);

	}
}
