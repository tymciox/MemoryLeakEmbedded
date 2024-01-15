#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include <windows.h>

#include "interface.h"
#include "memory_leak.h"

#define FILE_NAME "example.txt"

static FILE *file;

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

void open_file(void)
{
    file = fopen(FILE_NAME, "w");
    if (file == NULL)
        print("Error opening the file.\n");

    print("Memory Leak file opened\n");
}

void close_file(void)
{
    fclose(file);
    print("Memory Leak file closed\n");
}

void write_file(const char *single_info, const unsigned int len)
{
    fwrite(single_info, sizeof(char), len, file);
    print("Data written to file successfully.\n");
}
