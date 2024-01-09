#include <stdlib.h>
#include "memory_leak.h"

int main() {
	void *p1, *p2, *p3, *p4, *p5, *p6;

	p1 = malloc(10);
	p2 = malloc(20); // Memory Leak
	p3 = malloc(30);
	p4 = malloc(40); // Memory Leak
	p5 = malloc(50);
	p6 = malloc(60); // Memory Leak
	
	free(p1);
	free(p3);
	free(p5);

	memory_leak_print_result();
}