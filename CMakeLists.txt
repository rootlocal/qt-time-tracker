cmake_minimum_required(VERSION 3.23)

set(PACKAGE_MAJOR 0)
set(PACKAGE_MINOR 1)
set(PACKAGE_PATCH 2)

project(time_tracker
        VERSION "${PACKAGE_MAJOR}.${PACKAGE_MINOR}.${PACKAGE_PATCH}"
        LANGUAGES C CXX
        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pthread")

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/app/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/app/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/app/bin)

message(STATUS "COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
elseif (MSVC)
    add_compile_options(/MP)
elseif (MSVC OR MSYS OR MINGW)
    # for detecting Windows compilers
endif ()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${PROJECT_SOURCE_DIR}/ui")
set(CMAKE_AUTOUIC ON)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(GetGitHash)

set(LANGUAGES ru)

set(TS_DIR "${PROJECT_SOURCE_DIR}/resources/translations")

find_program(LUPDATE_EXECUTABLE lupdate)
find_program(LRELEASE_EXECUTABLE lrelease)

if (${LUPDATE_EXECUTABLE} STREQUAL "")
    find_program(LUPDATE_EXECUTABLE lupdate-qt5)
endif ()

if (${LRELEASE_EXECUTABLE} STREQUAL "")
    find_program(LRELEASE_EXECUTABLE lrelease-qt5)
endif ()

foreach (LANG ${LANGUAGES})

    set(TS_FILE "${TS_DIR}/application_${LANG}.ts")
    set(TS_BINARY "${TS_DIR}/application_${LANG}.qm")
    set(TRANSLATIONS ${TRANSLATIONS} ${TS_FILE})
    set(TRANSLATIONS_BINARY ${TRANSLATIONS_BINARY} ${TS_BINARY})

    if (NOT ${LUPDATE_EXECUTABLE} STREQUAL "")
        message(STATUS "Generate Translation language ${LANG} ...")
        execute_process(COMMAND ${LUPDATE_EXECUTABLE} -recursive ${CMAKE_SOURCE_DIR} -ts ${TS_FILE})
    endif ()

    if (NOT ${LRELEASE_EXECUTABLE} STREQUAL "")
        message(STATUS "Compile Translation language ${LANG} ...")
        execute_process(COMMAND ${LRELEASE_EXECUTABLE} ${TS_FILE})
    endif ()

endforeach ()

if (WIN32)
    set(GUI_TYPE WIN32)
    # for Windows operating system in general
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
    set(APP_ICON_RESOURCE_WINDOWS "${PROJECT_BINARY_DIR}/generated/resources.rc")
    configure_file("${PROJECT_SOURCE_DIR}/resources.rc.in" "${PROJECT_BINARY_DIR}/generated/resources.rc" @ONLY NEWLINE_STYLE UNIX)
    install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    # Make this a GUI application on Windows
    set(CMAKE_WIN32_EXECUTABLE ON)
endif ()

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)