#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(ProductionCode)
{
    RUN_TEST_CASE(ProductionCode, TwoMemoryLeak);
    RUN_TEST_CASE(ProductionCode, DoubleFree);
    RUN_TEST_CASE(ProductionCode, FreeNULL);
    RUN_TEST_CASE(ProductionCode, MallocError);
}