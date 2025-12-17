

set(PROTOWIRE_PROJECT_DEFAULT_OBJECT_KIND "LIBRARY" CACHE STRING "Default object kind")

macro(_check_not_opt NAME KIND OBJNAME)
    if(${opt_${NAME}})
        message(FATAL_ERROR "add_object(${OBJNAME} ...): Option not supported for ${KIND} objects: ${NAME}")
    endif()
endmacro()

macro(_check_not_alias_opt NAME OBJNAME)
    if(${opt_${NAME}})
        message(FATAL_ERROR "add_object(${OBJNAME} ...): Option not supported for alias: ${NAME}")
    endif()
endmacro()

function(add_object NAME)
    set(_library_options STATIC SHARED MODULE OBJECT INTERFACE)
    set(_exec_options WIN32 MACOSX_BUNDLE)
    set(_single_value ALIAS)
    set(_multi_value PRIVATE PUBLIC SYSTEM_PRIVATE SYSTEM_PUBLIC PRIVATE_INCLUDE PUBLIC_INCLUDE)
    set(_options LIBRARY EXEC NOT_GLOBAL EXCLUDE_FROM_ALL ${_library_options} ${_exec_options})
    cmake_parse_arguments(PARSE_ARGV 1 opt "${_options}" "${_single_value}" "${_multi_value}")

    if(${opt_LIBRARY} AND ${opt_EXEC})
        message(FATAL_ERROR "add_object(${NAME} ...): Must be called with at most one of LIBRARY or EXEC")
    elseif(${opt_LIBRARY})
        set(KIND LIBRARY)
    elseif(${opt_EXEC})
        set(KIND EXEC)
    else()
        set(KIND ${PROTOWIRE_PROJECT_DEFAULT_OBJECT_KIND})
    endif()

    foreach(_value ${opt_UNPARSED_ARGUMENTS})
        cmake_path(ABSOLUTE_PATH _value NORMALIZE OUTPUT_VARIABLE _last_path)
        if(NOT EXISTS ${_last_path})
            message(FATAL_ERROR "add_object(${NAME} ...): Source file not found: ${_last_path} (${_value})")
        endif()
    endforeach()

    if(${KIND} STREQUAL "EXEC")
        foreach(_optname ${_library_options})
            _check_not_opt(${_optname} EXEC ${NAME})
        endforeach()
        if(DEFINED opt_ALIAS)
            foreach(_optname ${_exec_options})
                _check_not_alias_opt(${_optname} ${NAME})
            endforeach()
            add_executable(${NAME} ALIAS ${opt_ALIAS})
        else()
            foreach(_optname ${_exec_options})
                if(opt_${_optname})
                    set(opt_${_optname} ${_optname})
                else()
                    set(opt_${_optname})
                endif()
            endforeach()
            add_executable(${NAME} ${opt_WIN32} ${opt_MACOSX_BUNDLE} ${opt_UNPARSED_ARGUMENTS})
            set(_shared_visibility PRIVATE)
        endif()
    elseif(${KIND} STREQUAL "LIBRARY")
        foreach(_optname ${_exec_options})
            _check_not_opt(${_optname} LIBRARY ${NAME})
        endforeach()
        if(DEFINED opt_ALIAS)
            foreach(_optname ${_library_options})
                _check_not_alias_opt(${_optname} ${NAME})
            endforeach()
            add_library(${NAME} ALIAS ${opt_ALIAS})
        else()
            foreach(_optname ${_library_options})
                if(opt_${_optname})
                    set(opt_${_optname} ${_optname})
                else()
                    set(opt_${_optname})
                endif()
            endforeach()
            add_library(${NAME} ${opt_STATIC} ${opt_SHARED} ${opt_MODULE} ${opt_INTERFACE} ${opt_UNPARSED_ARGUMENTS})
            set(_shared_visibility PUBLIC)
        endif()
    else()
        message(FATAL_ERROR "add_object(${NAME} ...): Unsunpported object kind: ${KIND}")
    endif()

    if(NOT DEFINED opt_ALIAS)
        target_include_directories(${NAME} ${_shared_visibility}
            ${PROJECT_SOURCE_DIR}/include ## header includes
        )
        target_include_directories(${NAME} PRIVATE
            ${PROJECT_SOURCE_DIR}/source ## object includes
        )
        target_compile_definitions(${NAME} PRIVATE
            "_OBJECT_NAME=$<TARGET_PROPERTY:${NAME},NAME>"
            "_OBJECT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}"
            "_OBJECT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}"
        )
    endif()

    if(DEFINED opt_PRIVATE)
        target_link_libraries(${NAME} PRIVATE ${opt_PRIVATE})
    endif()

    if(DEFINED opt_PUBLIC)
        target_link_libraries(${NAME} PUBLIC ${opt_PUBLIC})
    endif()

    if(DEFINED opt_SYSTEM_PRIVATE)
        target_link_libraries(${NAME} PRIVATE ${opt_SYSTEM_PRIVATE})
    endif()

    if(DEFINED opt_SYSTEM_PUBLIC)
        target_link_libraries(${NAME} PUBLIC ${opt_SYSTEM_PUBLIC})
    endif()


    if(DEFINED opt_PRIVATE_INCLUDE)
        target_include_directories(${NAME} PRIVATE ${opt_PRIVATE_INCLUDE})
    endif()

    if(DEFINED opt_PUBLIC_INCLUDE)
        target_include_directories(${NAME} PUBLIC ${opt_PUBLIC_INCLUDE})
    endif()

endfunction()
