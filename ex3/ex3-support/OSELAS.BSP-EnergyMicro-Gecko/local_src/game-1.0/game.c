
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 1
#define _GNU_SOURCE 1
#endif
#include <fcntl.h>
#include <signal.h>
#include "pong.h"

void input_handler();

uint32_t button_value;
uint32_t last_button_value;
FILE* gamepadDriver;
FILE* framebufferDriver;

bool handling;
int main(int argc, char *argv[])
{
	handling = false;
	int oflags;
	printf("Hello World, I'm game!\n");
	gamepadDriver = fopen("dev/GamepadDriver", "r");
	framebufferDriver = fopen("dev/fb0", "rb+");
	if (signal(SIGIO, &input_handler)== SIG_ERR){
		printf("ERROR1 \n");
	} /* dummy sample; sigaction() is better */ 
	if(fcntl(fileno(gamepadDriver), F_SETOWN, getpid()) == -1){
		printf("ERROR2 \n");
	}
	if (oflags = fcntl(fileno(gamepadDriver), F_GETFL) == -1){
		printf("ERROR3 \n");
	}
	if (fcntl(fileno(gamepadDriver), F_SETFL, oflags | FASYNC) == -1){
		printf("ERROR4 \n");
	}
	
	fprintf("Getpid: %x \n",getpid());
	
	last_button_value = 0;
	
	enterGame(framebufferDriver);
	printf("Game done");
	
	fclose(gamepadDriver);
	fclose(framebufferDriver);
	exit(EXIT_SUCCESS);
}

void input_handler(){

	if(handling) return;
	handling = true;

	int t = (int) fgets(&button_value, 4, gamepadDriver);

	button_value = 0xffffffff ^ button_value;

	onKeyDown((last_button_value ^ button_value) & button_value);
	onKeyUp((last_button_value ^ button_value) & last_button_value);

	last_button_value = button_value;
	handling = false;
}
