
add_executable(test_fortcpp tests.C testMain.C)
add_executable(bench_fortcpp benchmark.C)
target_link_libraries(test_fortcpp PUBLIC FortCpp)
target_link_libraries(bench_fortcpp PUBLIC FortCpp benchmark)

add_test(NAME test_fortcpp COMMAND ./test_fortcpp)
add_test(NAME bench_fortcpp COMMAND ./bench_fortcpp)
