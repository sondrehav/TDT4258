#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

void input_handler(uint32_t value);


int main(int argc, char *argv[])
{
	
	printf("Hello World, I'm game!\n");
	FILE* file = fopen("dev/GamepadDriver", "r+w+");
	uint32_t button_value;
	
	signal(SIGIO, &input_handler); /* dummy sample; sigaction() is better */ 
	fcntl(STDIN_FILENO, F_SETOWN, getpid());
	int oflags = fcntl(STDIN_FILENO, F_GETFL);
	fcntl(STDIN_FILENO, F_SETFL, oflags | FASYNC);
	
	fprintf(file, "Test for driver!");
	
	int i = 0;
	while (i<30) {
		i++; 
		sleep(1);
	}

	fclose(file);
	exit(EXIT_SUCCESS);
}

void input_handler(){
	int t = (int) fgets((void*) button_value, 4, file);
	printf("Program: %x\n", button_value);
}
