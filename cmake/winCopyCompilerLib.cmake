
macro(win_copy_compiler_dll APP DLL_NAME)

    get_filename_component(COMPILER_DIRECTORY ${CMAKE_CXX_COMPILER} DIRECTORY)
    find_file("DLL_NAME_PATH_${DLL_NAME}"
            NAMES "${DLL_NAME}"
            PATHS "${COMPILER_DIRECTORY}"
            NO_DEFAULT_PATH
            NO_CACHE
            REQUIRED
            )

    message(STATUS "COPY ${DLL_NAME}: ${DLL_NAME_PATH_${DLL_NAME}}")

    add_custom_command(TARGET ${APP} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DLL_NAME_PATH_${DLL_NAME}}" $<TARGET_FILE_DIR:${APP}>)
endmacro()
