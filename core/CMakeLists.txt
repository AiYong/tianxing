cmake_minimum_required(VERSION 3.8)
project(tianxing)

include_directories(/usr/include /usr/local/include .)
set(CMAKE_CXX_STANDARD 17)
aux_source_directory(. SRC_LIST)

add_library(tianxing ${SRC_LIST})