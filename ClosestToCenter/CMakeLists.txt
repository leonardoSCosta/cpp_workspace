cmake_minimum_required(VERSION 3.16.3)
project(ClosestToCenter)

set(CMAKE_CXX_STANDARD 11)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
add_executable(ClosestToCenter closestToCenter.cpp)

# Link the OSQP shared library
target_link_libraries(ClosestToCenter Eigen3::Eigen)
