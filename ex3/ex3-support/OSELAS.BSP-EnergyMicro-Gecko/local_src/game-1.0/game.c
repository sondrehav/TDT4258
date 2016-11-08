#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>


void input_handler();
uint32_t button_value;
uint32_t last_button_value;
FILE* file;

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

void (*function_pointers[8]) (void);

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
	function_pointers[0] = b1;
	function_pointers[1] = b2;
	function_pointers[2] = b3;
	function_pointers[3] = b4;
	last_button_value = 0;
	int i = 0;
	while (i<100) {
		i++; 
		sleep(1);
	}

	fclose(file);
	exit(EXIT_SUCCESS);
}

void input_handler(){
	int t = (int) fgets(&button_value, 4, file);
	int i = 0;
	while (i<8){
		uint32_t mask = (0x1 << i);
		if(((button_value & mask) == mask) && ((last_button_value & mask) != mask)){
			(*function_pointers[i])();
		}
		i++;
	}

	last_button_value = button_value;
}
