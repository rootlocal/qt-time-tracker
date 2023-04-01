cmake_minimum_required(VERSION 3.23)
project(time_tracker)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/app/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/app/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/app/bin)

set(CMAKE_CXX_STANDARD 17)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${PROJECT_SOURCE_DIR}/ui")
set(CMAKE_AUTOUIC ON)


set(LANGUAGES ru)
set(TS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources/translations")

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

# Find Libraries
find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE RESOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.qrc")
file(GLOB_RECURSE UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui")

qt5_wrap_ui(UI_HEADERS ${UI_FILES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${HEADERS}
        ${RESOURCE_FILES}
        ${UI_HEADERS}
        )

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Widgets)

add_custom_target(translations ALL DEPENDS ${TRANSLATIONS_BINARY} ${TRANSLATIONS_QT})
add_custom_target(resources ALL DEPENDS ${RESOURCE_FILES})
add_dependencies(resources translations)
add_dependencies(${PROJECT_NAME} resources)