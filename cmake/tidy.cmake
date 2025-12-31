
if(NOT DEFINED ${CMAKE_PROJECT_NAME}_LINT)
    include(${PROJECT_SOURCE_DIR}/cmake/lint.cmake)
endif()

set(TIDY_LANGUAGES "C;CXX")

set(${CMAKE_PROJECT_NAME}_TIDY_CXX_ARGS
    "--header-filter=${PROJECT_SOURCE_DIR}/source,${PROJECT_SOURCE_DIR}/include,${PROJECT_SOURCE_DIR}/tests;--checks=boost-*,llvm-*"
    CACHE STRING
    "clang-tidy args (C++)")

function (check_tidy)
    get_property(_lang_all GLOBAL PROPERTY ENABLED_LANGUAGES)

    set(${CMAKE_PROJECT_NAME}_CHECK_TIDY_CXX ${${CMAKE_PROJECT_NAME}_LINT} CACHE BOOL
        "Use clang-tidy on project C++ sources")

    if(${${CMAKE_PROJECT_NAME}_CHECK_TIDY_CXX}
            AND NOT DEFINED CMAKE_CXX_CLANG_TIDY)
        ## note that the find_program call will use/set a cache variable for the provided var
        find_program(TIDY_PROGRAM_CXX NAMES clang-tidy OPTIONAL
            DOC "Filesystem pathname for clang-tidy (C++)")
        if(${TIDY_PROGRAM_CXX} STREQUAL "TIDY_PROGRAM_CXX-NOTFOUND")
            message(WARNING "${CMAKE_PROJECT_NAME}_CHECK_TIDY_CXX is enabled but clang-tidy was not found. Set TIDY_PROGRAM_CXX to the clang-tidy binary pathname for linting with clang-tidy")
        else()
            set(CMAKE_CXX_CLANG_TIDY
                "${TIDY_PROGRAM_CXX};${${CMAKE_PROJECT_NAME}_TIDY_CXX_ARGS}" CACHE STRING
                "shell command for clang-tidy (C++)")
        endif()
    endif()
endfunction()
