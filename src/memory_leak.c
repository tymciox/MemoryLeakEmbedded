#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "memory_leak.h"
#include "interface.h"

#ifdef MEMORY_LEAK_EMBEDDED

#undef malloc
#undef calloc
#undef free

static memory_interface_t interface = {
    .print = print,
    .get_thread_name = get_thread_name,
    .get_time = get_time,
    .mutex_create = mutex_create,
    .mutex_give = mutex_give,
    .mutex_take = mutex_take,
    .open = file_open,
    .write = file_write,
    .close = file_close,
};

static memory_leak_node_t *FirstNodePtr = NULL;
static int FirstUseLibrary = 1;

static void memory_leak_add_info(void *mem_address, const unsigned int time, const unsigned int size, const char *file, const unsigned int line, const char *thread_name, const leak_status_t error);
static int memory_leak_remove_single_info(void *mem_address);
static void memory_leak_get_string_error(char *retbuf, const leak_status_t status);

// replacement of malloc
void *xmalloc(const unsigned int size, const char *file, const unsigned int line)
{
    void *allocation_ptr = NULL;
    char thread_name[THREAD_NAME_LENGTH] = {0};
    unsigned int time = 0;

    interface.get_thread_name(thread_name);
    interface.get_time(&time);

    allocation_ptr = malloc(size);

    if (allocation_ptr != NULL)
        memory_leak_add_info(allocation_ptr, time, size, file, line, thread_name, LEAK_OK);
    else
        memory_leak_add_info(allocation_ptr, time, size, file, line, thread_name, LEAK_MALLOC_ERROR);

    return allocation_ptr;
}

// replacement of calloc
void *xcalloc(const unsigned int elements, const unsigned int size, const char *file, const unsigned int line)
{
    unsigned int total_size = 0;
    char thread_name[THREAD_NAME_LENGTH] = {0};
    unsigned int time = 0;
    void *allocation_ptr = NULL;

    allocation_ptr = calloc(elements, size);
    if (allocation_ptr != NULL)
    {
        total_size = elements * size;
        memory_leak_add_info(allocation_ptr, time, total_size, file, line, thread_name, LEAK_OK);
    }
    else
        memory_leak_add_info(allocation_ptr, time, total_size, file, line, thread_name, LEAK_MALLOC_ERROR);

    return allocation_ptr;
}

// replacement of free
void xfree(void *mem_address, const char *file, const unsigned int line)
{
    leak_status_t status = LEAK_OK;
    char thread_name[THREAD_NAME_LENGTH] = {0};
    unsigned int time = 0;

    status = memory_leak_remove_single_info(mem_address);
    if (status == LEAK_OK)
        free(mem_address);
    else
        memory_leak_add_info(mem_address, time, 0, file, line, thread_name, status);
}

static void init_library(void)
{
    interface.mutex_create();
    FirstUseLibrary = 0;
}

static void memory_leak_add_info(void *mem_address, const unsigned int time, const unsigned int size, const char *file, const unsigned int line, const char *thread_name, const leak_status_t error)
{
    memory_leak_node_t *new_mem_node = NULL;

    if (FirstUseLibrary == 1)
        init_library();

    new_mem_node = (memory_leak_node_t *)calloc(1, sizeof(memory_leak_node_t));
    if (new_mem_node == NULL)
        return;

    /* fill up the structure with all info */
    new_mem_node->mem_info.address = mem_address;
    new_mem_node->mem_info.time = time;
    new_mem_node->mem_info.size = size;
    strncpy(new_mem_node->mem_info.file_name, file, FILE_NAME_LENGTH);
    new_mem_node->mem_info.line = line;
    strncpy(new_mem_node->mem_info.thread_name, thread_name, THREAD_NAME_LENGTH);
    new_mem_node->mem_info.error = error;
    new_mem_node->next = NULL;

    interface.mutex_take();
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
    interface.mutex_give();
}

static int memory_leak_remove_single_info(void *mem_address)
{
    memory_leak_node_t *iter = NULL;

    interface.mutex_take();
    iter = FirstNodePtr;
    if (iter == NULL || mem_address == NULL)
    {
        interface.mutex_give();
        return LEAK_FREE_NULL;
    }

    if (mem_address == iter->mem_info.address)
    {
        FirstNodePtr = iter->next;
        free(iter);
        interface.mutex_give();
        return LEAK_OK;
    }

    while (iter->next != NULL)
    {
        if (mem_address == iter->next->mem_info.address)
        {
            memory_leak_node_t *to_delete = iter->next;
            iter->next = iter->next->next;
            free(to_delete);
            interface.mutex_give();
            return LEAK_OK;
        }
        iter = iter->next;
    }

    interface.mutex_give();

    return LEAK_FREE_NULL;
}

bool memory_leak_is_error_occurs(const leak_status_t status)
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

void memory_leak_clear_all(void)
{
    memory_leak_node_t *temp = NULL;
    memory_leak_node_t *alloc_info = NULL;

    interface.mutex_take();
    temp = FirstNodePtr;
    alloc_info = FirstNodePtr;

    while (alloc_info != NULL)
    {
        alloc_info = alloc_info->next;
        free(temp);
        temp = alloc_info;
    }
    FirstNodePtr = NULL;
    interface.mutex_give();
}

static void memory_leak_get_string_error(char *retbuf, const leak_status_t status)
{
    strcpy(retbuf, "Warning !!! ERROR: ");
    switch (status)
    {
    case LEAK_MALLOC_ERROR:
        strcat(retbuf, "LEAK_MALLOC_ERROR\n");
        break;
    case LEAK_FREE_NULL:
        strcat(retbuf, "LEAK_FREE_NULL\n");
        break;
    default:
        strcat(retbuf, "UNKNOW ERROR\n");
        break;
    }
}

void memory_leak_print_result(void)
{
    char single_info[50 + FILE_NAME_LENGTH + THREAD_NAME_LENGTH] = {0};

    interface.print("Memory Leak Summary\n");
    interface.print("************************\n");

    interface.mutex_take();
    for (memory_leak_node_t *leak_info = FirstNodePtr; leak_info != NULL; leak_info = leak_info->next)
    {
        if (leak_info->mem_info.error != 0)
        {
            memory_leak_get_string_error(single_info, leak_info->mem_info.error);
            interface.print(single_info);
            memset(single_info, 0, sizeof(single_info));
        }

        sprintf(single_info, "%d,%d,%p,%s,%d,%s\n",
                leak_info->mem_info.time, leak_info->mem_info.size,
                leak_info->mem_info.address, leak_info->mem_info.file_name,
                leak_info->mem_info.line, leak_info->mem_info.thread_name);

        interface.print(single_info);

        memset(single_info, 0, sizeof(single_info));
    }
    interface.mutex_give();
    interface.print("***********************\n");
}

void memory_leak_write_result_to_a_file(const char *filename)
{
    char single_info[50 + FILE_NAME_LENGTH + THREAD_NAME_LENGTH] = {0};

    interface.open(filename);

    interface.mutex_take();
    for (memory_leak_node_t *leak_info = FirstNodePtr; leak_info != NULL; leak_info = leak_info->next)
    {
        if (leak_info->mem_info.error != 0)
        {
            memory_leak_get_string_error(single_info, leak_info->mem_info.error);
            interface.write(single_info, strlen(single_info));
            memset(single_info, 0, sizeof(single_info));
        }

        sprintf(single_info, "%d,%d,%p,%s,%d,%s\n",
                leak_info->mem_info.time, leak_info->mem_info.size,
                leak_info->mem_info.address, leak_info->mem_info.file_name,
                leak_info->mem_info.line, leak_info->mem_info.thread_name);

        interface.write(single_info, strlen(single_info));

        memset(single_info, 0, sizeof(single_info));
    }
    interface.mutex_give();
    interface.close();
}

#endif