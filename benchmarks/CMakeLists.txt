add_executable(gqe_bench benchmark.cpp)
target_link_libraries(gqe_bench PRIVATE gqe_lib)
target_compile_options(gqe_bench PRIVATE -Wall -Wextra -Wpedantic)