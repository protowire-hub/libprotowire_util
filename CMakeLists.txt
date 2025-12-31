
## cmake 3.20 defines cmake_path
cmake_minimum_required (VERSION 3.20)

project(libprotowire_util CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

if(POLICY CMP0167)
  # for FindBoost
  #
  # this may not affect the CMP0167 message showing up
  # during vcpkg build
  cmake_policy(SET CMP0167 NEW)
endif()


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(BOOTSTRAP_VCPKG ON CACHE BOOL
  "Bootsrap vcpkg if true and vendor/vcpkg/vcpkg does not exist")

set(PROTOWIRE_UTIL_TESTS ON CACHE BOOL
  "Build tests")

if(DEFINED ENV{BUILD_TESTS})
  message(STATUS "BUILD_TESTS defined in environment. Setting PROTOWIRE_UTIL_TESTS=$ENV{BUILD_TESTS}")
  set(PROTOWIRE_UTIL_TESTS $ENV{BUILD_TESTS})
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmake/tools_dir.cmake)
install_compiler_tools()

# FIXME add a docs note to README:
#
# Given the content of CMakePresets.json, bootstrapping vcpkg
# with the tooling provided here would require running
# cmake, at most once, without any presets

include(${CMAKE_CURRENT_LIST_DIR}/cmake/bootstrap_vcpkg.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/add_vendored.cmake)

add_subdirectory(source)

if(PROTOWIRE_UTIL_TESTS)
  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()
