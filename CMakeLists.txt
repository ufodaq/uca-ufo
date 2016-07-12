cmake_minimum_required(VERSION 2.6)
project(ucaufo C)

set(UCAUFO_VERSION "0.0.1")


list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(PkgConfig)
include(PkgConfigVars)

add_definitions("-std=c99 -Wall")
add_definitions(-DG_LOG_DOMAIN="Uca-Ufo")

pkg_check_modules(UCA libuca>=2.0.0 REQUIRED)
pkg_check_modules(GIO2 gio-2.0>=2.24 REQUIRED)
pkg_check_modules(PCITOOL pcitool>=0.2.0 REQUIRED)
pkg_check_variable(libuca plugindir)


set(CPACK_SOURCE_GENERATOR "TBZ2")
set(CPACK_PACKAGE_CONTACT "Suren A. Chilingaryan <csa@suren.me>")
set(CPACK_PACKAGE_VERSION "${UCAUFO_VERSION}")
set(CPACK_SOURCE_IGNORE_FILES "/.git/;/.bzr/;CMakeFiles;_CPack_Packages;cmake_install.cmake;CPack.*.cmake;CMakeCache.txt;install_manifest.txt;config.h$;.pc$;Makefile;.tar.bz2$;~$;${CPACK_SOURCE_IGNORE_FILES}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}")
include(CPack)

add_custom_target(dist_clean COMMAND ${CMAKE_MAKE_PROGRAM} clean WORKING_DIRECTORY ${CMAKE_CURRENT_DIR})
add_custom_target(dist DEPENDS dist_clean COMMAND ${CMAKE_MAKE_PROGRAM} package_source)


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

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ucaufo.spec.in 
	       ${CMAKE_CURRENT_BINARY_DIR}/ucaufo.spec)

include_directories(
    ${UCA_INCLUDE_DIRS}
    ${PCITOOL_INCLUDE_DIRS}
    ${GIO2_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR})

add_library(ucaufo SHARED uca-ufo-camera.c)

target_link_libraries(ucaufo
    ${UCA_LIBRARIES}
    ${PCITOOL_LIBRARIES}
    ${GIO2_LIBRARIES})

install(TARGETS ucaufo
    LIBRARY DESTINATION ${LIBUCA_PLUGINDIR})
