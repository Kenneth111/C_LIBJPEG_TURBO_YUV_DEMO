cmake_minimum_required(VERSION 3.7)
project(test_jpg_turbo)

set(CMAKE_C_STANDARD 99)
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)
set(SOURCE_FILES main.c tj_test.c yuv_test.c)
add_executable(test_jpg_turbo ${SOURCE_FILES})
target_link_libraries(test_jpg_turbo -lm libturbojpeg)