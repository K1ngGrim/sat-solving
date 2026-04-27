cmake_minimum_required(VERSION 3.16)
project(blatt)

set(CMAKE_CXX_STANDARD 14)

add_executable(blatt_1_1
        ${CMAKE_SOURCE_DIR}/src/solver.cpp
        ${CMAKE_SOURCE_DIR}/src/cadical/src/ipasir.cpp
)

target_include_directories(blatt_1_1 PRIVATE
        src/cadical/src
)

target_link_libraries(blatt_1_1
        ${CMAKE_SOURCE_DIR}/src/cadical/build/libcadical.a
)