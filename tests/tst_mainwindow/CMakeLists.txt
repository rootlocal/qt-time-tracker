cmake_minimum_required(VERSION 3.23)
project(tst_mainwindow)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable AUTOMOC before add_executable
set(CMAKE_AUTOMOC ON)

# Find Libraries
find_package(Qt5 COMPONENTS Widgets Core Sql Test REQUIRED)

# Collect test sources
file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB TEST_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/../../config.h.in" "${PROJECT_BINARY_DIR}/generated/config.h")

# Optional: avoid accidentally compiling stale generated files
list(FILTER TEST_SOURCES EXCLUDE REGEX "/moc_.*\\.cpp$")
list(FILTER TEST_SOURCES EXCLUDE REGEX "/ui_.*\\.cpp$")
list(FILTER TEST_SOURCES EXCLUDE REGEX "/qrc_.*\\.cpp$")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include)
include_directories("${PROJECT_BINARY_DIR}/generated")

file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/../../include/*.h")
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "main.cpp")

# Create the test executable
add_executable(${PROJECT_NAME}
        ${TEST_SOURCES}
        ${TEST_HEADERS}
        ${HEADERS}
        ${SOURCES}
        )

# Collect link libraries and link once using the keyword signature
set(_tst_libs
        Qt5::Core
        Qt5::Widgets
        Qt5::Sql
        Qt5::Test
        )

target_link_libraries(${PROJECT_NAME} PRIVATE ${_tst_libs})

# Register the test with CTest
add_test(NAME ${PROJECT_NAME} COMMAND tst_mainwindow)
