#ifndef INTERFACE_MEMORY_LEAK_H
#define INTERFACE_MEMORY_LEAK_H

typedef void (*print_fun)(char *string_info);
typedef void (*get_thread_name_fun)(char *thread_name);
typedef void (*get_time_fun)(unsigned int *time);
typedef void (*open_file_fun)(void);
typedef void (*write_file_fun)(const char *data, const unsigned int len);
typedef void (*close_file_fun)(void);

typedef struct memory_interface
{
    print_fun print;
    get_thread_name_fun get_thread_name;
    get_time_fun get_time;
    open_file_fun open;
    write_file_fun write;
    close_file_fun close;
} memory_interface_t;

void print(char *string_info);
void get_thread_name(char *thread_name);
void get_time(unsigned int *time);

void open_file(void);
void close_file(void);
void write_file(const char *single_info, const unsigned int len);

#endif