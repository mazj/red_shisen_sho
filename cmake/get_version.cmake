find_package(Git REQUIRED)

function(getVersion version date)
    if (GIT_EXECUTABLE)
        execute_process(COMMAND ${GIT_EXECUTABLE} describe --tag
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            RESULT_VARIABLE status
            OUTPUT_VARIABLE PROJECT_VERSION
            ERROR_QUIET
            )
        if(${status} OR ${PROJECT_VERSION} EQUAL "")
            set(PROJECT_VERSION "0.0.0-0")
            message("Error determining application version!!!")
        else()
            string(STRIP ${PROJECT_VERSION} PROJECT_VERSION)
            string(REGEX MATCH "[0-9]+\.[0-9]+\.[0-9]+(-[0-9+]+)?" PROJECT_VERSION ${PROJECT_VERSION})
            if (${CMAKE_MATCH_0})
                set(PROJECT_VERSION "0.0.0-0")
            endif()
        endif()

        execute_process(COMMAND ${GIT_EXECUTABLE} show -s --format=%ci
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            RESULT_VARIABLE status
            OUTPUT_VARIABLE PROJECT_DATE
            ERROR_QUIET
            )
        if(${status})
            set(PROJECT_DATE "0000-00-00 00:00:00 +0000")
        else()
            string(STRIP ${PROJECT_DATE} PROJECT_DATE)
        endif()

        set(${version} ${PROJECT_VERSION} PARENT_SCOPE)
        set(${date} ${PROJECT_DATE} PARENT_SCOPE)
    endif()
endfunction()

function(write_version_file VERSION_FILE PROG_VERSION PROG_DATE)
    file(WRITE ${VERSION_FILE}
        "// Do not edit this file, created automatically.\n"
        "static const QString program_version = \"${PROG_VERSION} ${PROG_DATE}\";"
        )
    message("Created new version file ${VERSION_FILE}.")
endfunction()
