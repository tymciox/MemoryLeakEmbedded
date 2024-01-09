add_library(Unity STATIC
cunity/unity.c
cunity/unity_fixture.c
)

target_include_directories(Unity PUBLIC
cunity/
)