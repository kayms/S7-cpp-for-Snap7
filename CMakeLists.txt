include(ExternalProject)
cmake_minimum_required(VERSION 3.1)

project(Snap7)

set(CMAKE_CXX_STANDARD 11)

# Architektur auswählen
set(SNAP7_ARCH "aarch64_linux")
set(SNAP7_ARCH_LIB_DIR "aarch64-linux")

set(SNAP7_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/snap7-full-1.4.2)
set(SNAP7_MAKEFILE ${SNAP7_SOURCE_DIR}/build/unix/${SNAP7_ARCH}.mk)
set(SNAP7_LIB ${SNAP7_SOURCE_DIR}/build/bin/${SNAP7_ARCH_LIB_DIR}/libsnap7.so)
set(SNAP7_INCLUDE_DIR ${SNAP7_SOURCE_DIR}/src/lib ${SNAP7_SOURCE_DIR}/src/core ${SNAP7_SOURCE_DIR}/src/sys)

# Externes Snap7-Build-Target
ExternalProject_Add(snap7_project
    SOURCE_DIR ${SNAP7_SOURCE_DIR}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} -C ${SNAP7_SOURCE_DIR}/build/unix -f ${SNAP7_MAKEFILE} all
    INSTALL_COMMAND ""
    BUILD_BYPRODUCTS ${SNAP7_LIB}
)

file(GLOB Snap7_SRC
    "*.h"
    "*.cpp"
)

add_library(Snap7 ${Snap7_SRC})
add_dependencies(Snap7 snap7_project)
target_include_directories(Snap7 PRIVATE ${SNAP7_INCLUDE_DIR})
target_link_libraries(Snap7 PRIVATE ${SNAP7_LIB})