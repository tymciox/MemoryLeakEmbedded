#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <Windows.h>

#include "memory_leak.h"

#define UNUSED_VARIABLE(var) (void)(var);

static void *first_thread_function(void *arg)
{
	void *p1, *p2, *p3, *p4;
	UNUSED_VARIABLE(arg);

	p1 = malloc(1); // Memory Leak
	p2 = malloc(2); // Memory Leak
	p3 = malloc(3); // Memory Leak
	p4 = malloc(4); // Memory Leak

	free(p1);
	UNUSED_VARIABLE(p2);
	free(p3);
	UNUSED_VARIABLE(p4);

	return NULL;
}

static void *second_thread_function(void *arg)
{
	void *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8;
	UNUSED_VARIABLE(arg);

	p1 = malloc(110);
	Sleep(10);
	p2 = malloc(120); // Memory Leak
	Sleep(10);
	p3 = malloc(130);
	Sleep(10);
	p4 = malloc(140); // Memory Leak
	Sleep(10);
	p5 = malloc(110); // Memory Leak
	Sleep(10);
	p6 = malloc(120); // Memory Leak
	Sleep(10);
	p7 = malloc(130); // Memory Leak
	Sleep(10);
	p8 = malloc(140);

	free(p1);
	UNUSED_VARIABLE(p2);
	free(p3);
	UNUSED_VARIABLE(p4);
	UNUSED_VARIABLE(p5);
	UNUSED_VARIABLE(p6);
	UNUSED_VARIABLE(p7);
	free(p8);

	return NULL;
}

int main()
{
	void *p1, *p2, *p3, *p4, *p5, *p6;
	pthread_t thread, thread_2; // Thread identifier

	// Create a new thread
	pthread_create(&thread, NULL, first_thread_function, NULL);

	// Create a new thread
	pthread_create(&thread_2, NULL, second_thread_function, NULL);

	p1 = malloc(10);
	Sleep(10);
	p2 = malloc(20); // Memory Leak
	Sleep(10);
	p3 = malloc(30);
	Sleep(10);
	p4 = malloc(40); // Memory Leak
	Sleep(10);
	p5 = malloc(50);
	Sleep(10);
	p6 = malloc(60); // Memory Leak

	free(p1);
	UNUSED_VARIABLE(p2);
	free(p3);
	UNUSED_VARIABLE(p4);
	free(p5);
	UNUSED_VARIABLE(p6);

	Sleep(500);
	memory_leak_print_result();
}