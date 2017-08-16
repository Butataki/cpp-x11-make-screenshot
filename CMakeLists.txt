cmake_minimum_required(VERSION 3.8)
project(cpp-x11-make-screenshot)
set(CMAKE_BUILD_TYPE Release)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_library(x11screenshot STATIC src/screenshot)
target_link_libraries(x11screenshot X11 png jpeg)
set_property(TARGET x11screenshot PROPERTY CXX_STANDARD 11)

add_library(x11cmdopts STATIC src/cmdopts)
set_property(TARGET x11cmdopts PROPERTY CXX_STANDARD 11)

add_executable(makeshot src/main.cpp)
target_link_libraries(makeshot X11 x11screenshot x11cmdopts)
set_property(TARGET makeshot PROPERTY CXX_STANDARD 11)

# install(TARGETS x11screenshot DESTINATION /usr/lib)
# install(FILES screenshot.h DESTINATION include)
install(TARGETS makeshot DESTINATION bin)
