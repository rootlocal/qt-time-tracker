
find_package(Git)

if (GIT_FOUND)
    message(STATUS "Git found: ${GIT_EXECUTABLE} version: ${GIT_VERSION_STRING}")

    # Get the latest abbreviated commit hash of the working branch
    execute_process(
            COMMAND ${GIT_EXECUTABLE} log -1 --format=%h
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            OUTPUT_VARIABLE GIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    message(STATUS "Current git hash ${CMAKE_PROJECT_NAME}: ${GIT_HASH}")
else ()
    message(STATUS "Git not found")
endif ()