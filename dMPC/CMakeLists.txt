cmake_minimum_required(VERSION 3.5)
project(DMPC)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra)
  # add_compile_options(-Wall -Wextra -Wpedantic) # Muitos warnings devido ao -Wpedantic
endif()

find_package(Eigen3 3.4 REQUIRED NO_MODULE)
find_package(fmt)
add_executable(DMPC dMPC.cpp)

# Link the OSQP shared library
target_link_libraries(DMPC Eigen3::Eigen)
target_link_libraries(DMPC fmt::fmt)
