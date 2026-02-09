# cmake_minimum_required(VERSION 3.7)
# project(SDL2Test)
#
# find_package(SDL2 REQUIRED)
# include_directories(${SDL2_INCLUDE_DIRS})
#
# # find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
# # find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
# # include_directories(SDL2Test ${SDL2_INCLUDE_DIRS})
# message(${SDL2_INCLUDE_DIRS})
# message(${SDL2_LIBRARIES})
#
# add_executable(SDL2Test audio.cpp)
# # target_link_libraries(SDL2Test PRIVATE SDL2::SDL2main)
# # target_link_libraries(SDL2Test PRIVATE SDL2::SDL2)
# target_link_libraries(SDL2Test ${SDL2_LIBRARIES})
#
cmake_minimum_required(VERSION 3.7)

project(SDL2Test)

include(FindPkgConfig)
pkg_check_modules(SDL2_Mixer REQUIRED IMPORTED_TARGET SDL2_mixer)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(SDL2Test audio.cpp)
target_link_libraries(SDL2Test SDL2::SDL2 PkgConfig::SDL2_Mixer)
