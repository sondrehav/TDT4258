#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>


void input_handler();
uint32_t button_value;
FILE* file;
int last_button_value;

int main(int argc, char *argv[])
{	
	last_button_value = 0;
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

void b1(){
	printf("Button1\n");
}

void b2(){
	printf("Button2\n");	
}

void b3(){
	printf("Button3\n");
}

void b4(){
	printf("Button4\n");
}

void input_handler(){
	int t = (int) fgets(&button_value, 4, file);
	if(((button_value & 0x1) == 0x1) && ((last_button_value & 0x1) != 0x1)) {
		b1();
	}
	if(((button_value & 0x2) == 0x2) && ((last_button_value & 0x2) != 0x2)) {
		b2();
	}
	if(((button_value & 0x4) == 0x4) && ((last_button_value & 0x4) != 0x4)) {
		b3();
	}
	if(((button_value & 0x8) == 0x8) && ((last_button_value & 0x8) != 0x8)) {
		b4();
	}

	last_button_value = button_value;
}
