find_package(Threads REQUIRED)

add_library(gqe_lib graph.cpp algorithms.cpp thread_pool.cpp wal.cpp)
target_include_directories(gqe_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_options(gqe_lib PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(gqe_lib PUBLIC Threads::Threads)

add_executable(gqe main.cpp)
target_link_libraries(gqe PRIVATE gqe_lib)