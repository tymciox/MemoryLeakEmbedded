typedef void (*print_function)(char *string_info);

typedef struct memory_interface
{
    print_function print;
} memory_interface_t;

void print(char *string_info);