#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");
	FILE* file = fopen("dev/GamepadDriver", "r+w+");
	char buf[256];

	fprintf(file, "Test for driver!");
	
	int i = 0;
	while (i<30) {
		printf("Buf before : %x\n" ,buf);
		int t = (int) fgets(buf, 256 ,file);
		printf("Program: %s\n", buf);
		printf("1?: %d\n", t);
		int j = 0;
		while (j < 17){
			printf("%d 	%c \n", buf[j], buf[j]);
			j++;
			}
		printf("Buf after : %x\n" ,buf);
		i++; 
		sleep(1);
	}
	

	fclose(file);
	exit(EXIT_SUCCESS);
}
