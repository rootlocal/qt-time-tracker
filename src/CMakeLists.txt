cmake_minimum_required(VERSION 3.23)

# Find Libraries
find_package(Qt5 COMPONENTS Widgets Core Sql REQUIRED)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${PROJECT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE RESOURCE_FILES "${PROJECT_SOURCE_DIR}/resources/*.qrc")
file(GLOB_RECURSE UI_FILES "${PROJECT_SOURCE_DIR}/ui/*.ui")

qt5_wrap_ui(UI_HEADERS ${UI_FILES})
configure_file("${PROJECT_SOURCE_DIR}/config.h.in" "${PROJECT_BINARY_DIR}/generated/config.h")
include_directories("${PROJECT_BINARY_DIR}/generated")
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${HEADERS}
        ${RESOURCE_FILES}
        ${UI_HEADERS}
        ${APP_ICON_RESOURCE_WINDOWS}
        )

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Widgets Qt5::Sql)

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
            ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/scripts
            ${CMAKE_BINARY_DIR}/app/scripts
            COMMENT "Copy Script")

    add_dependencies(${PROJECT_NAME} copy_script)

    add_custom_target(copy_icons ALL
            ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/resources/icons
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