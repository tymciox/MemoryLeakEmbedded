#ifndef INTERFACE_MEMORY_LEAK_H
#define INTERFACE_MEMORY_LEAK_H

typedef void (*print_fun)(char *string_info);
typedef void (*get_thread_name_fun)(char *thread_name);
typedef void (*get_time_fun)(unsigned int *time);

typedef struct memory_interface
{
    print_fun print;
    get_thread_name_fun get_thread_name;
    get_time_fun get_time;
} memory_interface_t;

void print(char *string_info);
void get_thread_name(char *thread_name);
void get_time(unsigned int *time);

#endif