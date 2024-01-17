#ifndef INTERFACE_MEMORY_LEAK_H
#define INTERFACE_MEMORY_LEAK_H

typedef void (*print_fun)(char *string_info);
typedef void (*get_thread_name_fun)(char *thread_name);
typedef void (*get_time_fun)(unsigned int *time);
typedef void (*mutex_create_fun)(void);
typedef void (*mutex_give_fun)(void);
typedef void (*mutex_take_fun)(void);
typedef void (*file_open_fun)(const char *filename);
typedef void (*file_write_fun)(const char *data, const unsigned int len);
typedef void (*file_close_fun)(void);

typedef struct memory_interface
{
    get_time_fun get_time;
    print_fun print;
    get_thread_name_fun get_thread_name;
    mutex_create_fun mutex_create;
    mutex_give_fun mutex_give;
    mutex_take_fun mutex_take;
    file_open_fun open;
    file_write_fun write;
    file_close_fun close;
} memory_interface_t;

void print(char *string_info);
void get_thread_name(char *thread_name);
void get_time(unsigned int *time);

void mutex_create(void);
void mutex_give(void);
void mutex_take(void);

void file_open(const char *filename);
void file_close(void);
void file_write(const char *single_info, const unsigned int len);

#endif