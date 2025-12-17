
include(${CMAKE_CURRENT_LIST_DIR}/add_object.cmake)

function(add_catch_test test_name)
    if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/include)
        set(_test_includes ${CMAKE_CURRENT_LIST_DIR}/include)
    else()
        set(_test_includes)
    endif()

    add_object(${test_name} EXEC ${test_name}.cpp
        PRIVATE_INCLUDE ${_test_includes}
        SYSTEM_PRIVATE Catch2::Catch2WithMain
        ${ARGN}
    )

    catch_discover_tests(${test_name})
endfunction()
