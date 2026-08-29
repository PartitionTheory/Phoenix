add_executable(phoenix_clock_test phoenix_clock_test.c)
target_include_directories(phoenix_clock_test PRIVATE ../../include)
target_link_libraries(phoenix_clock_test phoenix_clock)

