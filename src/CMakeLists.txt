add_library(gqe_lib graph.cpp)
target_include_directories(gqe_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_options(gqe_lib PRIVATE -Wall -Wextra -Wpedantic)

add_executable(gqe main.cpp)
target_link_libraries(gqe PRIVATE gqe_lib)