cmake_minimum_required(VERSION 3.7)

# Dependencies
# sudo apt install libcppdb-dev libcppdb-postgresql0  

project(PostgresTest)

find_library(CPPDB_LIB cppdb)
if(NOT CPPDB_LIB)
    message(FATAL_ERROR "cppdb library not found")
endif()

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${CPPDB_LIB})
