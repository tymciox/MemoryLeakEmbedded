cmake_minimum_required(VERSION 3.22)

project(memory_leak)

add_compile_options(-Wall -Wextra)

add_subdirectory(test)

set(INCLUDE_DIR
    .
)

set(INCLUDE_FFF
    test/fff
    src/
)

include_directories(${INCLUDE_DIR} ${INCLUDE_FFF})

add_executable(
    ${CMAKE_PROJECT_NAME}
    src/memory_leak.c
    src/interface.c
    test/TestProductionCode_main.c
    test/TestProductionCode_Runner.c
    test/TestProductionCode.c
)

target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    Unity
)
