#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include <windows.h>

#include "interface.h"
#include "memory_leak.h"

void print(char *string_info)
{
    printf("%s", string_info);
}

void get_thread_name(char *destination_thread_name)
{
    pthread_t thread_id = pthread_self();

    snprintf(destination_thread_name, THREAD_NAME_LENGTH, "Thread %lld", thread_id);
}

void get_time(unsigned int *destination_time)
{
    *destination_time = GetTickCount();
}

void open_file()
{
}

void close_file()
{
}

void write_to_file()
{
}
