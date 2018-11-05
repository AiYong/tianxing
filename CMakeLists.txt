cmake_minimum_required(VERSION 3.8)
project(tianxing)


set(CMAKE_CXX_STANDARD 17)
link_directories(/usr/lib /usr/local/lib)
add_subdirectory(core)
add_executable(tianxing_test main.cpp)
target_link_libraries(tianxing_test /Users/aiyong/Projects/tianxing/cmake-build-debug/core/libtianxing.a)
target_link_libraries(tianxing_test boost_program_options)
target_link_libraries(tianxing_test boost_context-mt)