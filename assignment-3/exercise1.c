#include <stdio.h>

int magic_function();

int main (void)
{
	int start = 1;
	magic_function();

	int stack_space = (int)(&magic_function - &main);

	printf("Memory used: %d\n", stack_space);
}
