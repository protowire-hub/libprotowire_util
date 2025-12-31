
set(COMPILER_TOOLS_DIR ${CMAKE_CURRENT_BINARY_DIR}/compiler_tools CACHE STRING
  "Directory for compiler tools during build")

function(read_shell_path)
    file(READ ${CMAKE_CURRENT_BINARY_DIR}/.path_string __SHELL_PATH )
    set(ENV{PATH} ${__SHELL_PATH})
endfunction()

macro(shell_prepend_path DIR)
    ## FIXME this does not apply any test on uniqueness

    ## note that the GENRATE command may fail during cmake call,
    ## if the output file was created directly within CMAKE_CURRENT_BINARY_DIR
    ##
    ## COMPILER_TOOLS_DIR will be created within install_compiler_tools()

    ## FIXME cmake defers the write unitl after the read !!
    file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/.path_string
        CONTENT "$<SHELL_PATH:${DIR};${CMAKE_PROGRAM_PATH};$ENV{PATH}>")

    ## broken
    cmake_language(DEFER CALL read_shell_path)
endmacro()

macro(install_compiler_tools)
    # known limitation: not tested for cross-compiling

    if(NOT EXISTS ${COMPILER_TOOLS_DIR})
        file(MAKE_DIRECTORY ${COMPILER_TOOLS_DIR})
    endif()

    if(DEFINED CMAKE_C_COMPILER_LAUNCHER)
        ## this assumes that each compiler launcher uses a semantics similar to ccache,
        ## such that e.g a symlink to the compiler launcher will be processed as representing
        ## the basename (file name) of the compiler under $PATH, excepting the PATH entry
        ## denoting the symbolic link itself.

        if(NOT DEFINED CMAKE_C_COMPILER)
            set(CMAKE_C_COMPILER cc)
        endif()

        get_filename_component(C_COMPILER_BASENAME ${CMAKE_C_COMPILER} NAME)

        find_program(C_COMPILER_LAUNCHER_PATHNAME ${CMAKE_C_COMPILER_LAUNCHER})

        if(NOT EXISTS ${COMPILER_TOOLS_DIR}/cc)
            file(CREATE_LINK  ${C_COMPILER_LAUNCHER_PATHNAME} ${COMPILER_TOOLS_DIR}/cc   SYMBOLIC)
        endif()


        if(NOT EXISTS ${COMPILER_TOOLS_DIR}/${C_COMPILER_BASENAME})
            file(CREATE_LINK ${C_COMPILER_LAUNCHER_PATHNAME}
                    ${COMPILER_TOOLS_DIR}/${C_COMPILER_BASENAME}
                     SYMBOLIC)
        endif()
    endif()

    if(DEFINED CMAKE_CXX_COMPILER_LAUNCHER)
        ## assumptions are similar to above
        get_filename_component(CXX_COMPILER_BASENAME ${CMAKE_CXX_COMPILER} NAME)

        find_program(CXX_COMPILER_LAUNCHER_PATHNAME ${CMAKE_CXX_COMPILER_LAUNCHER})

        if(NOT EXISTS ${COMPILER_TOOLS_DIR}/cxx)
            file(CREATE_LINK ${CXX_COMPILER_LAUNCHER_PATHNAME} ${COMPILER_TOOLS_DIR}/cxx  SYMBOLIC)
        endif()

        if(NOT EXISTS ${COMPILER_TOOLS_DIR}/${CXX_COMPILER_BASENAME})
            file(CREATE_LINK ${CXX_COMPILER_LAUNCHER_PATHNAME}
                    ${COMPILER_TOOLS_DIR}/${CXX_COMPILER_BASENAME}
                     SYMBOLIC)
        endif()
    endif()

    # # shell_prepend_path(${COMPILER_TOOLS_DIR})
    # ## still fails (in read!) on a nonexistent file wtf?
    # cmake_language(DEFER CALL shell_prepend_path ${COMPILER_TOOLS_DIR})
    set(ENV{CC} ${COMPILER_TOOLS_DIR}/${CXX_COMPILER_BASENAME})
    set(ENV{CXX} ${COMPILER_TOOLS_DIR}/${CXX_COMPILER_BASENAME})
endmacro()
