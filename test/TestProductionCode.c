#include <stdio.h>

#include "unity.h"
#include "unity_fixture.h"
#include "fff.h"

#include "memory_leak.h"

#define UNUSED_VARIABLE(var) (void)(var);

DEFINE_FFF_GLOBALS;

TEST_GROUP(ProductionCode);

TEST_SETUP(ProductionCode)
{
    memory_leak_clear_all();
}

TEST_TEAR_DOWN(ProductionCode)
{
}

TEST(ProductionCode, TwoMemoryLeak)
{
    int *mem = malloc(100);
    int *mem1 = malloc(110);
    int *mem2 = malloc(120);
    int *mem3 = malloc(130); // memory leak
    int *mem4 = malloc(140);
    int *mem5 = malloc(150); // memory leak

    free(mem);
    free(mem1);
    free(mem2);
    free(mem4);

    (void)(mem3); // memory leak
    (void)(mem5); // memory leak

    memory_leak_write_result_to_a_file("example1.txt");

    TEST_ASSERT_EQUAL_INT(2, memory_leak_get_counter());
}

TEST(ProductionCode, DoubleFree)
{
    int *mem = malloc(100);

    free(mem);
    free(mem);

    TEST_ASSERT_TRUE(memory_leak_is_error_occurs(LEAK_FREE_NULL));
}

TEST(ProductionCode, FreeNULL)
{
    int *mem = NULL;

    free(mem);

    TEST_ASSERT_TRUE(memory_leak_is_error_occurs(LEAK_FREE_NULL));
}

TEST(ProductionCode, MallocError)
{
    int *mem = NULL;

    mem = calloc(-10, -1);
    (void)mem;

    TEST_ASSERT_TRUE(memory_leak_is_error_occurs(LEAK_MALLOC_ERROR));
}
