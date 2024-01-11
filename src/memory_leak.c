#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "memory_leak.h"

#ifdef MEMORY_LEAK_DETECTOR

#undef malloc
#undef calloc
#undef free

static memory_leak_node_t *FirstNodePtr = NULL;

static void memory_leak_add_info(void *mem_address, unsigned int size, const char *file, unsigned int line, leak_status_t error)
{
    memory_leak_node_t *new_mem_node = NULL;

    new_mem_node = (memory_leak_node_t *)calloc(1, sizeof(memory_leak_node_t));
    if (new_mem_node == NULL)
        return;

    /* fill up the structure with all info */
    new_mem_node->mem_info.address = mem_address;
    new_mem_node->mem_info.size = size;
    strncpy(new_mem_node->mem_info.file_name, file, FILE_NAME_LENGTH);
    new_mem_node->mem_info.line = line;
    new_mem_node->mem_info.error = error;
    new_mem_node->next = NULL;

    if (FirstNodePtr == NULL)
    {
        FirstNodePtr = new_mem_node;
    }
    else
    {
        memory_leak_node_t *iter = FirstNodePtr;
        while (iter->next != NULL)
            iter = iter->next;
        iter->next = new_mem_node;
    }
}

static int memory_leak_remove_single_info(void *mem_address)
{
    memory_leak_node_t *iter = FirstNodePtr;

    if (iter == NULL || mem_address == NULL)
        return LEAK_FREE_NULL;

    if (mem_address == iter->mem_info.address)
    {
        FirstNodePtr = iter->next;
        free(iter);
        return LEAK_OK;
    }

    while (iter->next != NULL)
    {
        if (mem_address == iter->next->mem_info.address)
        {
            memory_leak_node_t *to_delete = iter->next;
            iter->next = iter->next->next;
            free(to_delete);
            return LEAK_OK;
        }
        iter = iter->next;
    }
    return LEAK_DOUBLE_FREE;
}

void memory_leak_clear_all(void)
{
    memory_leak_node_t *temp = NULL;
    memory_leak_node_t *alloc_info = NULL;

    temp = FirstNodePtr;
    alloc_info = FirstNodePtr;

    while (alloc_info != NULL)
    {
        alloc_info = alloc_info->next;
        free(temp);
        temp = alloc_info;
    }
}

// replacement of malloc
void *xmalloc(unsigned int size, const char *file, unsigned int line)
{
    void *allocation_ptr = NULL;

    allocation_ptr = malloc(size);
    if (allocation_ptr != NULL)
        memory_leak_add_info(allocation_ptr, size, file, line, LEAK_OK);
    else
        memory_leak_add_info(allocation_ptr, size, file, line, LEAK_MALLOC_ERROR);

    return allocation_ptr;
}

// replacement of calloc
void *xcalloc(unsigned int elements, unsigned int size, const char *file, unsigned int line)
{
    unsigned int total_size = 0;
    void *allocation_ptr = calloc(elements, size);
    if (allocation_ptr != NULL)
    {
        total_size = elements * size;
        memory_leak_add_info(allocation_ptr, total_size, file, line, LEAK_OK);
    }
    else
        memory_leak_add_info(allocation_ptr, total_size, file, line, LEAK_MALLOC_ERROR);

    return allocation_ptr;
}

// replacement of free
void xfree(void *mem_address, const char *file, unsigned int line)
{
    leak_status_t status = LEAK_OK;

    status = memory_leak_remove_single_info(mem_address);
    if (status == LEAK_OK)
        free(mem_address);
    else
        memory_leak_add_info(mem_address, 0, file, line, status);
}

bool memory_leak_is_error_occurs(leak_status_t status)
{
    for (memory_leak_node_t *leak_info = FirstNodePtr; leak_info != NULL; leak_info = leak_info->next)
    {
        if (leak_info->mem_info.error == (unsigned int)status)
            return true;
    }
    return false;
}

int memory_leak_get_counter(void)
{
    int counter = 0;
    for (memory_leak_node_t *leak_info = FirstNodePtr; leak_info != NULL; leak_info = leak_info->next)
    {
        counter++;
    }
    return counter;
}

static void memory_leak_print_error(leak_status_t status)
{
    printf("Warning !!! ERROR: ");
    switch (status)
    {
    case LEAK_MALLOC_ERROR:
        printf("LEAK_MALLOC_ERROR\n");
        break;
    case LEAK_DOUBLE_FREE:
        printf("LEAK_DOUBLE_FREE\n");
        break;
    case LEAK_FREE_NULL:
        printf("LEAK_FREE_NULL\n");
        break;
    default:
        printf("UNKNOW ERROR - %d\n", status);
        break;
    }
}

void memory_leak_print_result(void)
{
    printf("%s\n", "Memory Leak Summary");
    printf("%s\n", "************************");

    for (memory_leak_node_t *leak_info = FirstNodePtr; leak_info != NULL; leak_info = leak_info->next)
    {
        if (leak_info->mem_info.error != 0)
            memory_leak_print_error(leak_info->mem_info.error);
        printf("%d,", leak_info->mem_info.time);
        printf("%d,", leak_info->mem_info.size);
        printf("%p,", leak_info->mem_info.address);
        printf("%s,", leak_info->mem_info.file_name);
        printf("%d,", leak_info->mem_info.line);
        printf("%s\n", leak_info->mem_info.thread_name);
    }

    printf("%s\n", "***********************");
    // #format, : "time,size,address,source_file_name,line_number,thread_name"
    // memory_leak_clear_all();
}

#endif