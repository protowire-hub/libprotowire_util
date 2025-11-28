# cmake include file vendor/vcpkg
#
# - provides conditional initialization for the vendor/vcpkg submodule
# - runs bootstrap_vcpkg.{sh, bat}  if vendor/vcpkg/vcpkg does not exist
# - will not run tests or shell commands if BOOTSTRAP_VCPKG is false

if(BOOTSTRAP_VCPKG)
  set(BOOTSTRAP_NEED_GIT OFF)
  if(NOT EXISTS ${PROJECT_SOURCE_DIR}/vendor/vcpkg/.git)
    include(FindGit)
    find_package(Git OPTIONAL)
    if(Git_FOUND)
      message(STATUS "Initializing vendor/vcpkg submodule")
      execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init vendor/vcpkg
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND_ERROR_IS_FATAL ANY
      )
    else()
      set(BOOTSTRAP_NEED_GIT ON)
    endif()
  endif()
  if(NOT EXISTS ${PROJECT_SOURCE_DIR}/vendor/vcpkg/vcpkg)
    if(BOOTSTRAP_NEED_GIT)
      message(FATAL_ERROR "vendor/vcpkg not initialized. Git not found. Unable to bootstrap vcpkg")
    else()
      if(WIN32)
        set(SCRIPT_EXT bat)
      else()
        set(SCRIPT_EXT sh)
      endif()
      set(VCPKG_BOOTSTRAP_SCRIPT ${PROJECT_SOURCE_DIR}/vendor/vcpkg/bootstrap-vcpkg.${SCRIPT_EXT} 
        CACHE STRING "Bootstrap script for vcpkg")
      message(STATUS "Bootstrapping vcpkg")
      execute_process(
        COMMAND ${VCPKG_BOOTSTRAP_SCRIPT}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/vendor/vcpkg
        RESULT_VARIABLE VCPKG_BOOSTRAP_RESULT
      )
      if(${VCPKG_BOOSTRAP_RESULT} EQUAL 0)
        message(FATAL_ERROR "Bootstrapped vcpkg. Please restart the cmake build")
      else()
        message(FATAL_ERROR "Failed to bootstrap vcpkg: ${VCPKG_BOOSTRAP_RESULT} [${VCPKG_BOOTSTRAP_SCRIPT}]")
      endif()
    endif() # BOOTSTRAP_NEED_GIT test
  endif() # vendor/vcpkg/vcpkg test
endif() # BOOTSTRAP_VCPKG
