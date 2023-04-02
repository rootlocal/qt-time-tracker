cmake_minimum_required(VERSION 3.23)

set(PACKAGE_MAJOR 0)
set(PACKAGE_MINOR 1)
set(PACKAGE_PATCH 1)

project(time_tracker
        VERSION "${PACKAGE_MAJOR}.${PACKAGE_MINOR}.${PACKAGE_PATCH}"
        LANGUAGES C CXX
        )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/app/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/app/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/app/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pthread")

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    message(STATUS ">>> COMPILER_ID: CLANG")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

elseif (MSVC)
    add_compile_options(/MP)
elseif (MSVC OR MSYS OR MINGW)
    # for detecting Windows compilers
endif ()

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${PROJECT_SOURCE_DIR}/ui")
set(CMAKE_AUTOUIC ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
include(GetGitHash)

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

# Find Libraries
find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE RESOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.qrc")
file(GLOB_RECURSE UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui")

qt5_wrap_ui(UI_HEADERS ${UI_FILES})
configure_file("${PROJECT_SOURCE_DIR}/config.h.in" "${PROJECT_BINARY_DIR}/generated/config.h")
include_directories("${PROJECT_BINARY_DIR}/generated")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${HEADERS}
        ${RESOURCE_FILES}
        ${UI_HEADERS}
        ${APP_ICON_RESOURCE_WINDOWS}
        )

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Widgets)

add_custom_target(translations ALL DEPENDS ${TRANSLATIONS_BINARY} ${TRANSLATIONS_QT})
add_custom_target(resources ALL DEPENDS ${RESOURCE_FILES})
add_dependencies(resources translations)
add_dependencies(${PROJECT_NAME} resources)

if (APPLE)
    # for MacOS X or iOS, watchOS, tvOS (since 3.10.3)
    # Run windeployqt Retrieve the absolute path to qmake and then use that path to find the binaries
    get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(MAC_DEPLOY_QT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")

    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
            env PATH="${_qt_bin_dir}" "${MAC_DEPLOY_QT_EXECUTABLE}"
            "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMENT "Running macdeployqt..."
    )
elseif (WIN32)
    add_custom_target(copy_script ALL
            ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/scripts
            ${CMAKE_BINARY_DIR}/app/scripts
            COMMENT "Copy Script")
    add_dependencies(${PROJECT_NAME} copy_script)

    add_custom_target(copy_icons ALL
            ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons
            ${CMAKE_BINARY_DIR}/app/icons
            COMMENT "Copy icons")
    add_dependencies(${PROJECT_NAME} copy_icons)

    # Run windeployqt Retrieve the absolute path to qmake and then use that path to find the binaries
    get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WIN_DEPLOY_QT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
            env PATH="${_qt_bin_dir}" "${WIN_DEPLOY_QT_EXECUTABLE}"
            "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMENT "Running windeployqt...")
endif ()

if (MSVC)
    include(InstallRequiredSystemLibraries)
elseif (MINGW)
    include(winCopyCompilerLib)
    win_copy_compiler_dll(${PROJECT_NAME} "libatomic-1.dll")
    win_copy_compiler_dll(${PROJECT_NAME} "libgcc_s_seh-1.dll")
    win_copy_compiler_dll(${PROJECT_NAME} "libgomp-1.dll")
    win_copy_compiler_dll(${PROJECT_NAME} "libquadmath-0.dll")
    win_copy_compiler_dll(${PROJECT_NAME} "libssp-0.dll")
    win_copy_compiler_dll(${PROJECT_NAME} "libstdc++-6.dll")
    win_copy_compiler_dll(${PROJECT_NAME} "libwinpthread-1.dll")
endif ()