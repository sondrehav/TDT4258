#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");
	FILE* file = fopen("dev/GamepadDriver", "w+");
	char buf[255];

	fprintf(file, "Test for driver!");

	int t = (int) fscanf(file, "%s", buf);
	printf("Program: %s\n", buf);
	printf("1?: %d\n", t);

	fclose(file);
	exit(EXIT_SUCCESS);
}
