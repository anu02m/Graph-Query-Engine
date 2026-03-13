add_executable(gqe_tests test_main.cpp)
target_link_libraries(gqe_tests PRIVATE gqe_lib GTest::gtest_main)
target_compile_options(gqe_tests PRIVATE -Wall -Wextra -Wpedantic)

add_test(NAME gqe_tests COMMAND gqe_tests)