# How it work ?
The library help to find memory leaks in embedded C.
It stores each unrealeased memory using a linked list.

# How to start
1. Copy `src/` directory to your project,
2. Uncomment the flag `#define MEMORY_LEAK_DETECTOR` inside the source file `memory_leak.h`,
3. Inlcude header file `#include "memory_leak.h"` in your code,
4. Adjust `src/interface.c` as you need,
4. While the program is running, you need to call a function: `memory_leak_print_result()`

# How to build unit test
1. In the main directory:
```
cmake -S . -B BuildTest
cd BuildTest
make
./memory_leak
```
