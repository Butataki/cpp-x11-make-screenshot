# General options
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

# CPack
INCLUDE(InstallRequiredSystemLibraries)
SET(CPACK_GENERATOR "DEB")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "makeshot 17.08.15.1")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "makeshot 17.08.15.1")
SET(CPACK_PACKAGE_NAME "makeshot")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Tool to make screenshots and save them to files")
SET(CPACK_PACKAGE_VENDOR "Butataki")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
SET(CPACK_PACKAGE_VERSION_MAJOR "17")
SET(CPACK_PACKAGE_VERSION_MINOR "08")
SET(CPACK_PACKAGE_VERSION_PATCH "15.1")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
SET(CPACK_PACKAGE_EXECUTABLES "makeshot")
SET(CPACK_SOURCE_GENERATOR "TGZ")
INCLUDE(CPack)
