cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME "Big_Number_Calculator")
project(${PROJECT_NAME} LANGUAGES C CXX)

if(UNIX AND NOT APPLE)
    set(OUTPUT_FILE_NAME "bignumcalc")
else()
    set(OUTPUT_FILE_NAME "BigNumCalc")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

if(UNIX AND NOT APPLE)
    set(GMP_LIB "${CMAKE_CURRENT_SOURCE_DIR}/libraries/unix/gmp/libgmp.so")
    set(MPFR_LIB "${CMAKE_CURRENT_SOURCE_DIR}/libraries/unix/mpfr/libmpfr.so")
elseif(WIN32)
    set(GMP_LIB "${CMAKE_CURRENT_SOURCE_DIR}/libraries/win/gmp/libgmp-10.dll")
    set(MPFR_LIB "${CMAKE_CURRENT_SOURCE_DIR}/libraries/win/mpfr/libmpfr-6.dll")
elseif(APPLE)
    set(GMP_LIB "${CMAKE_CURRENT_SOURCE_DIR}/libraries/mac/gmp/libgmp.dylib")
    set(MPFR_LIB "${CMAKE_CURRENT_SOURCE_DIR}/libraries/mac/mpfr/libmpfr.dylib")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

set(HEADERS
    src/common_functions.h
    src/mainwindow.h
    src/widgets_reimplement.h
    src/worker.h
)

set(SOURCES
    src/common_functions.cpp
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow_widgets.cpp
    src/widgets_reimplement.cpp
    src/worker.cpp
)

set(FORMS
    src/mainwindow.ui
)

set(RESOURCES
    resources/resource.qrc
)

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    enable_language(RC)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/resources/resource_win.rc")
    add_executable(${PROJECT_NAME} WIN32 ${HEADERS} ${SOURCES} ${FORMS} ${RESOURCES} ${APP_ICON_RESOURCE_WINDOWS})
else()
    add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${FORMS} ${RESOURCES})
endif()

#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libraries/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/libraries/include")

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${OUTPUT_FILE_NAME})

target_link_libraries(${PROJECT_NAME}
                      Qt${QT_VERSION_MAJOR}::Core 
                      Qt${QT_VERSION_MAJOR}::Gui 
                      Qt${QT_VERSION_MAJOR}::Widgets
                      ${GMP_LIB}
                      ${MPFR_LIB})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${GMP_LIB}
    ${MPFR_LIB}
    "${CMAKE_BINARY_DIR}")
