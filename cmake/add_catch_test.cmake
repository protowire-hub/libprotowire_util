
include(${CMAKE_CURRENT_LIST_DIR}/add_dep.cmake)

function(add_catch_test test_name)
    add_executable(${test_name} "${test_name}.cpp")
    add_dep(${test_name} SYSTEM AFTER PRIVATE Catch2::Catch2WithMain ${ARGN})
    target_include_directories(${test_name} PRIVATE 
        ${PROJECT_SOURCE_DIR}/include ## project includes
        ${CMAKE_CURRENT_LIST_DIR}/include ## tests includes
    )
    catch_discover_tests(${test_name})
endfunction()
