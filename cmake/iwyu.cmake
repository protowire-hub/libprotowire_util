## conditional support for enabling include-what-you-use (IWYU)
## when (1) ${CMAKE_PROJECT_NAME}_LINT and (2) IWYU is available

if(NOT DEFINED ${CMAKE_PROJECT_NAME}_LINT)
    include(${PROJECT_SOURCE_DIR}/cmake/lint.cmake)
endif()

set(IWYU_LANGUAGES "C;CXX")

function (check_iwyu)
    get_property(_lang_all GLOBAL PROPERTY ENABLED_LANGUAGES)
    foreach(_lang ${_lang_all})
        if(${_lang} IN_LIST IWYU_LANGUAGES)
            set(${CMAKE_PROJECT_NAME}_CHECK_IWYU_${_lang} ${${CMAKE_PROJECT_NAME}_LINT} CACHE BOOL
                "Use include-what-you-use on project ${_lang} sources")

            if(${${CMAKE_PROJECT_NAME}_CHECK_IWYU_${_lang}}
                    AND NOT DEFINED CMAKE_${_lang}_INCLUDE_WHAT_YOU_USE)
                ## note that the find_program call will use/set a cache variable for the provided var
                find_program(IWYU_PROGRAM_${_lang} NAMES include-what-you-use iwyu OPTIONAL
                    DOC "Filesystem pathname with optional args (CMake ';'-list) for include-what-you-use with ${_lang} sources")
                if(${IWYU_PROGRAM_${_lang}} STREQUAL "IWYU_PROGRAM_${_lang}-NOTFOUND")
                    message(WARNING "${CMAKE_PROJECT_NAME}_CHECK_IWYU is enabled but include-what-you-use (IWYU) was not found. Set IWYU_PROGRAM_${_lang} to the IWYU binary pathname to enable linting with this tool")
                else()
                    set(CMAKE_${_lang}_INCLUDE_WHAT_YOU_USE ${IWYU_PROGRAM_${_lang}} CACHE STRING
                        "shell command for include-what-you-use with ${_lang} sources")
                endif()
            endif()
        endif()
    endforeach()
endfunction()
