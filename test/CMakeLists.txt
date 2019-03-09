
add_executable(test_fortcpp tests.C testMain.C)
target_link_libraries(test_fortcpp PUBLIC FortCpp)

add_test(NAME test_fortcpp COMMAND ./test_fortcpp)
