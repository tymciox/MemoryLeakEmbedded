
#ifndef MEMORY_LEAK_H
#define MEMORY_LEAK_H

#define MEMORY_LEAK_DETECTOR

#ifdef MEMORY_LEAK_DETECTOR
#include <stdbool.h>

#define FILE_NAME_LENGTH 25
#define THREAD_NAME_LENGTH 25

typedef enum
{
    LEAK_OK,
    LEAK_MALLOC_ERROR,
    LEAK_FREE_NULL
} leak_status_t;

#define malloc(size) xmalloc(size, __FILE__, __LINE__)
#define calloc(elements, size) xcalloc(elements, size, __FILE__, __LINE__)
#define free(mem_address) xfree(mem_address, __FILE__, __LINE__)
// TODO Add your own function which use heap

typedef struct
{
    unsigned int time;
    void *address;
    unsigned int size;
    char file_name[FILE_NAME_LENGTH];
    unsigned int line;
    unsigned int error;
    char thread_name[THREAD_NAME_LENGTH];
} memory_leak_info_t;

typedef struct memory_leak_node
{
    memory_leak_info_t mem_info;
    struct memory_leak_node *next;
} memory_leak_node_t;

void *xmalloc(const unsigned int size, const char *file, const unsigned int line);
void *xcalloc(const unsigned int elements, const unsigned int size, const char *file, const unsigned int line);
void xfree(void *mem_address, const char *file, const unsigned int line);

void memory_leak_clear_all(void);
int memory_leak_get_counter(void);
bool memory_leak_is_error_occurs(const leak_status_t status);

void memory_leak_print_result(void);
void memory_leak_write_result_to_a_file(void);

#endif

#endif
