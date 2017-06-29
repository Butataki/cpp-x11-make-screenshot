cmake_minimum_required(VERSION 3.8)
add_executable(makeshot main.cpp)
target_link_libraries(makeshot X11 png jpeg)
set_property(TARGET makeshot PROPERTY CXX_STANDARD 11)
