cmake_minimum_required(VERSION 4.0)
project(blatt_1_1)
project(blatt_1_3)
project(blatt_1_4)

set(CMAKE_CXX_STANDARD 14)

add_executable(blatt_1_3
        aufgabe2/sudoku_sat.cpp
        )

add_executable(blatt_1_4
        aufgabe4/pythagorean.cpp)

add_executable(blatt_1_1
        aufgabe1/solver.cpp
)

#target_include_directories(blatt_1_1 PRIVATE
#        aufgabe1/cadical/src
#)

#target_link_libraries(blatt_1_1
#        ${CMAKE_SOURCE_DIR}/aufgabe1/cadical/build/libcadical.a
#)
