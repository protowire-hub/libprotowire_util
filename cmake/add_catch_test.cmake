
include(${CMAKE_CURRENT_LIST_DIR}/add_object.cmake)

function(add_catch_test test_name)
    set(_options CUSTOM_MAIN)
    set(_single_value)
    set(_multi_value PRIVATE PRIVATE_INCLUDE SYSTEM_PRIVATE SOURCES)
    cmake_parse_arguments(PARSE_ARGV 1 opt "${_options}" "${_single_value}" "${_multi_value}")

    if(NOT DEFINED opt_SOURCES)
        set(opt_SOURCES ${test_name}.cpp)
    endif()

    if(${opt_CUSTOM_MAIN})
        set(_catch2_target Catch2::Catch2)
    else()
        set(_catch2_target Catch2::Catch2WithMain)
    endif()

    if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/include)
        set(_test_includes ${CMAKE_CURRENT_LIST_DIR}/include)
    else()
        set(_test_includes)
    endif()

    foreach(flag ${_multi_value})
        if(NOT DEFINED opt_${flag})
            set(opt_${flag})
        endif()
    endforeach()

    add_object(${test_name} EXEC
        ${opt_SOURCES}
        ${opt_UNPARSED_ARGUMENTS}
        PRIVATE ${opt_PRIVATE} protowire_test_libs
        PRIVATE_INCLUDE  ${opt_PRIVATE_INCLUDE} ${_test_includes}
        SYSTEM_PRIVATE ${opt_SYSTEM_PRIVATE} ${_catch2_target}
    )

    catch_discover_tests(${test_name})
endfunction()
