## libprotowire_util tests

include(${PROJECT_SOURCE_DIR}/cmake/add_catch_test.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/add_object.cmake)

set(CATCH_DEVELOPMENT_BUILD ON)

find_package(Catch2 REQUIRED 3)
include(Catch)

find_package(Boost CONFIG REQUIRED
    COMPONENTS preprocessor)

add_catch_test(test_type_name PRIVATE protowire_util Boost::preprocessor)
add_subdirectory(util_tests)
