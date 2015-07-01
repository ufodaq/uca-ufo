# --- Common ------------------------------------------------------------------

cmake_minimum_required(VERSION 2.6)
project(ucapco C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(PkgConfig)
include(PkgConfigVars)

add_definitions("-std=c99 -Wall")

pkg_check_modules(UCA libuca>=2.0.0 REQUIRED)
pkg_check_modules(GIO2 gio-2.0>=2.24 REQUIRED)
pkg_check_variable(libuca plugindir)


# --- Plugin specific ---------------------------------------------------------

find_package(IPE REQUIRED)

include_directories(
    ${UCA_INCLUDE_DIRS}
    ${IPE_INCLUDE_DIRS}
    ${GIO2_INCLUDE_DIRS})

add_library(ucaufo SHARED uca-ufo-camera.c)

target_link_libraries(ucaufo
    ${UCA_LIBRARIES}
    ${GIO2_LIBRARIES}
    ${IPE_LIBRARIES})

install(TARGETS ucaufo
    LIBRARY DESTINATION ${LIBUCA_PLUGINDIR})
