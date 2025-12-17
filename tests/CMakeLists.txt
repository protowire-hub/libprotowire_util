## libprotowire_util tests

include(${PROJECT_SOURCE_DIR}/cmake/add_catch_test.cmake)

set(CATCH_DEVELOPMENT_BUILD ON)

find_package(Catch2 REQUIRED 3)
include(Catch)

add_catch_test(test_type_name PRIVATE protowire_util)
