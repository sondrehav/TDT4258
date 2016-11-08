#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>


void input_handler();
uint32_t button_value;
FILE* file;

int main(int argc, char *argv[])
{
	int oflags;
	printf("Hello World, I'm game!\n");
	file = fopen("dev/GamepadDriver", "r+w+");
	if (signal(SIGIO, &input_handler)== SIG_ERR){
		printf("ERROR1 \n");
	} /* dummy sample; sigaction() is better */ 
	if(fcntl(fileno(file), F_SETOWN, getpid()) == -1){
		printf("ERROR2 \n");
	}
	if (oflags = fcntl(fileno(file), F_GETFL) == -1){
		printf("ERROR3 \n");
	}
	if (fcntl(fileno(file), F_SETFL, oflags | FASYNC) == -1){
		printf("ERROR4 \n");
		}
	fprintf("Getpid: %x \n",getpid());
	fprintf(file, "Test for driver!");
	
	int i = 0;
	while (i<20) {
		i++; 
		sleep(1);
	}

	fclose(file);
	exit(EXIT_SUCCESS);
}

void input_handler(){
	int t = (int) fgets(&button_value, 4, file);
	printf("Program: %x\n", button_value);
}
