cmake_minimum_required(VERSION 2.6)
project(ucapco C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(PkgConfig)
include(PkgConfigVars)

add_definitions("-std=c99 -Wall")
add_definitions(-DG_LOG_DOMAIN="Uca-Ufo")

pkg_check_modules(UCA libuca>=2.0.0 REQUIRED)
pkg_check_modules(GIO2 gio-2.0>=2.24 REQUIRED)
pkg_check_modules(PCITOOL pcitool>=0.2.0 REQUIRED)
pkg_check_variable(libuca plugindir)


if (NOT DEFINED CMOSIS_SENSOR_WIDTH)
    set(CMOSIS_SENSOR_WIDTH "2048")
endif ()

if (NOT DEFINED CMOSIS_SENSOR_HEIGHT)
    set(CMOSIS_SENSOR_HEIGHT "1088")
endif ()

set(CMOSIS_SENSOR_WIDTH ${CMOSIS_SENSOR_WIDTH} CACHE STRING "Width of the sensor")
set(CMOSIS_SENSOR_HEIGHT ${CMOSIS_SENSOR_HEIGHT} CACHE STRING "Height of the sensor")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/config.h)

include_directories(
    ${UCA_INCLUDE_DIRS}
    ${PCITOOL_INCLUDE_DIRS}
    ${GIO2_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR})

link_directories(${UCA_LIBRARY_DIRS})

add_library(ucaufo SHARED uca-ufo-camera.c)

target_link_libraries(ucaufo
    ${UCA_LIBRARIES}
    ${PCITOOL_LIBRARIES}
    ${GIO2_LIBRARIES})

install(TARGETS ucaufo
    LIBRARY DESTINATION ${LIBUCA_PLUGINDIR})

add_executable(test tests/test.c)
target_link_libraries(test ucaufo)
