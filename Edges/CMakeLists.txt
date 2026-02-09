cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(EdgeDetector)

find_package(OpenCV REQUIRED)

add_executable(edge_detector main.cpp)
target_link_libraries(edge_detector ${OpenCV_LIBS})
