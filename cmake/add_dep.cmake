
## INTERFFACE_% properties without BEFORE or AFTER options
set(_dep_properties_call
        INCLUDE_DIRECTORIES
        LINK_DIRECTORIES COMPILE_OPTIONS
        PRECOMPILE_HEADERS
        LINK_LIBRARIES LINK_OPTIONS
        COMPILE_DEFINITIONS COMPILE_FEATURES)

## properties whose call accepts a SYSTEM option
set(_dep_properties_system INCLUDE_DIRECTORIES)

## ... with an AFTER option
set(_dep_properties_after INCLUDE_DIRECTORIES)

## ... with a BEFORE Option
set(_dep_properties_before INCLUDE_DIRECTORIES LINK_DIRECTORIES COMPILE_OPTIONS)


function(_call_property target uc_name visibility flags)
    string(TOLOWER ${uc_name} call_name)
    set(call_name target_${call_name})

    foreach(dep ${ARGN})
        string(FIND ${dep} "::" dep_ns_start)
        if(${dep_ns_start} STREQUAL  "-1")
            set(var_base_name ${dep}_${uc_name})
        else()
            string(SUBSTRING ${dep} 0 ${dep_ns_start} dep_base)
            string(REPLACE "::" "_" dep_var_prefix ${dep})
            set(var_name ${dep_var_prefix}_${uc_name})
            if(DEFINED ${var_name} AND NOT ("${${var_name}}" STREQUAL  ""))
                # not reached
                message(FATAL_ERROR "DEBUG 01")
                cmake_language(CALL ${call_name} ${target} ${flags} ${visibility} ${${var_name}})
            endif()
            set(var_base_name ${dep_base}_${uc_name})
        endif()
        if(DEFINED ${var_base_name} AND NOT ("${${var_base_name}}" STREQUAL  ""))
            ## not reached
            message(FATAL_ERROR "DEBUG 02")
            cmake_language(CALL ${call_name} ${target} ${flags} ${visibility} ${${var_base_name}})
        endif()

        # handle the target INTERFACE_... properties
        get_property(_prop_set TARGET ${dep} PROPERTY INTERFACE_${uc_name} SET)
        if(_prop_set)
            get_property(_prop_val TARGET ${dep} PROPERTY INTERFACE_${uc_name})
            # message(STATUS "P! cmake_language(CALL ${call_name} ${flags} ${target} ${visibility} ${_prop_val})" )
            cmake_language(CALL ${call_name} ${target}  ${flags} ${visibility} ${_prop_val})
        endif()
    endforeach()
endfunction()

function(add_dep tgt)
    set(options SYSTEM BEFORE AFTER)
    set(ov_args)
    set(mv_args PUBLIC PRIVATE)
    cmake_parse_arguments(PARSE_ARGV 1 _DEP "${options}" "${ov_args}" "${mv_args}")

    # FIXME fail if not one of PUBLIC or PRIVATE was provided

    # not addressed here, INTERFACE_% properties for each:
    # POSITION_INDEPENDENT_CODE
    # AUTOMOC_MACRO_NAMES
    # AUTOUIC_OPTIONS
    # CXX_MODULE_SETS
    # HEADER_SETS
    # HEADER_SETS_TO_VERIFY

    if(NOT "${_DEP_PUBLIC}" STREQUAL "")
        target_link_libraries(${tgt} PUBLIC ${_DEP_PUBLIC})
    endif()

    if(NOT "${_DEP_PRIVATE}" STREQUAL "")
        target_link_libraries(${tgt} PRIVATE ${_DEP_PRIVATE})
    endif()

    foreach(_prop ${_dep_properties_call})
        set(_prop_flags "")
        if(${_DEP_SYSTEM} AND (${_prop} IN_LIST _dep_properties_system))
            list(APPEND _prop_flags SYSTEM)
        endif()
        if(${_DEP_BEFORE} AND (${_prop} IN_LIST _dep_properties_before))
            list(APPEND _prop_flags BEFORE)
        endif()
        if(${_DEP_AFTER} AND (${_prop} IN_LIST _dep_properties_after))
            list(APPEND _prop_flags AFTER)
        endif()

        # message(STATUS "P? PUBLIC: ${tgt} ${_prop} PUBLIC \"${_prop_flags}\" ${_DEP_PUBLIC}")
        # message(STATUS "P? PRIVATE: ${tgt} ${_prop} PRIVATE \"${_prop_flags}\" ${_DEP_PRIVATE}")

        _call_property(${tgt} ${_prop} PUBLIC "${_prop_flags}" ${_DEP_PUBLIC})
        _call_property(${tgt} ${_prop} PRIVATE "${_prop_flags}" ${_DEP_PRIVATE})

    endforeach()

endfunction()
