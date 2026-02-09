cmake_minimum_required(VERSION 3.16.3)
project(PrSBC)
# execute_process(COMMAND shell -c "export OsqpEigen_DIR=$HOME/Repositorios/RobotNavigation/PrSBC/osqp")
# execute_process(COMMAND shell -c "export Eigen3_DIR=$HOME/Repositorios/RobotNavigation/PrSBC/src/eigen-3.4.0/")
# execute_process(COMMAND shell -c "echo $OsqpEigen_DIR")

set(CMAKE_CXX_STANDARD 11)

# Find OSQP library and headers
find_package(OsqpEigen REQUIRED)
# Find Eigen library and headers
# find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(PrSBC src/main.cpp)

# Link the OSQP shared library
target_link_libraries(PrSBC OsqpEigen::OsqpEigen)
target_link_libraries(PrSBC Eigen3::Eigen)
