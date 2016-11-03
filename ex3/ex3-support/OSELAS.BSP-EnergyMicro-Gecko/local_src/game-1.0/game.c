#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");
	FILE* file = fopen("dev/GamepadDriver", "r+w+");
	char buf[256];

	fprintf(file, "Test for driver!");
	
	int t = (int) fgets(buf, 256 ,file);

	
	
	while(){

	}
	printf("Program: %s\n", buf);
	printf("1?: %d\n", t);

	fclose(file);
	exit(EXIT_SUCCESS);
}
