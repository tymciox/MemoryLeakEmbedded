#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include <windows.h>

#include "interface.h"
#include "memory_leak.h"

#ifdef MEMORY_LEAK_EMBEDDED

static FILE *file = NULL;
static HANDLE mutex = NULL;

/**
 * Required, SysTick is recommended
 */
void get_time(unsigned int *destination_time)
{
    *destination_time = GetTickCount();
}

/**
 * Required only if you print the result, otherwise leave it empty
 */
void print(char *string_info)
{
    printf("%s", string_info);
}

/**
 * Optional
 */
void get_thread_name(char *destination_thread_name)
{
    pthread_t thread_id = pthread_self();
    snprintf(destination_thread_name, THREAD_NAME_LENGTH, "Thread %lld", thread_id);
}

/**
 * Required only if you use multiple threads, otherwise leave it empty
 */
void mutex_create(void)
{
    mutex = CreateMutex(NULL, FALSE, NULL);
    if (mutex == NULL)
        print("Error creating mutex.\n");
}

/**
 * Required only if you use multiple threads, otherwise leave it empty
 */
void mutex_give(void)
{
    ReleaseMutex(mutex);
}

/**
 * Required only if you use multiple threads, otherwise leave it empty
 */
void mutex_take(void)
{
    WaitForSingleObject(mutex, INFINITE);
}

/**
 * Required only if you are writing to a file, otherwise leave it empty
 */
void file_open(const char *filename)
{
    file = fopen(filename, "w");
    if (file == NULL)
        print("Error opening the file.\n");

    print("Memory Leak file opened\n");
}

/**
 * Required only if you are writing to a file, otherwise leave it empty
 */
void file_close(void)
{
    fclose(file);
    print("Memory Leak file closed\n");
}

/**
 * Required only if you are writing to a file, otherwise leave it empty
 */
void file_write(const char *single_info, const unsigned int len)
{
    fwrite(single_info, sizeof(char), len, file);
    print("Data written to file successfully.\n");
}

#endif