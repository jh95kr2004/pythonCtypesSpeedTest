cmake_minimum_required(VERSION 3.0)

project("pythonCtypesSpeedTest")

set(CMAKE_CXX_STANDARD 11)

file(GLOB srcs
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
)

add_library(pythonCtypesSpeedTest SHARED ${srcs})
target_link_libraries(pythonCtypesSpeedTest pthread)
set_target_properties(pythonCtypesSpeedTest PROPERTIES
    COMPILE_FLAGS "-Wall -O2 -fPIC"
)

add_executable(test ${PROJECT_SOURCE_DIR}/test/main.c)
target_link_libraries(test pythonCtypesSpeedTest)