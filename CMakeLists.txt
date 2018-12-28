cmake_minimum_required(VERSION 3.7)
project(test_jpg_turbo)

set(CMAKE_C_STANDARD 99)

set(BUILD_DIR ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_BINARY_DIR ${BUILD_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BUILD_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)
aux_source_directory(src SOURCE_FILES)
#set(SOURCE_FILES src/main.c src/tj_test.c src/yuv_test.c)
add_executable(test_jpg_turbo ${SOURCE_FILES})
target_link_libraries(test_jpg_turbo libturbojpeg)