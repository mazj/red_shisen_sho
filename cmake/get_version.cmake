find_package(Git REQUIRED)

function(getVersion version date)
    if (GIT_EXECUTABLE)
        execute_process(COMMAND ${GIT_EXECUTABLE} describe --tag --match "[0-9]*.[0-9]*.[0-9]*"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            RESULT_VARIABLE status
            OUTPUT_VARIABLE PROJECT_VERSION
            ERROR_QUIET
            )
        if(${status})
            set(PROJECT_VERSION "0.0.0-0")
        else()
            string(STRIP ${PROJECT_VERSION} PROJECT_VERSION)
            string(REGEX MATCH "[0-9]*.[0-9]*.[0-9]*-[0-9]*" PROJECT_VERSION ${PROJECT_VERSION})
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
