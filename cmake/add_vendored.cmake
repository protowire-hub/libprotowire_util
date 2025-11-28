
include(FetchContent)

macro(add_vendored_submodule name vendor)
  set(VENDOR_SUBMODULE_${name} vendor/${vendor} CACHE INTERNAL "vendor sumbmodule for ${name}")
  set(VENDOR_SOURCE_DIR_${name} ${PROJECT_SOURCE_DIR}/${VENDOR_SUBMODULE_${name}} CACHE INTERNAL "vendored source dir for ${name}")

  FetchContent_Declare(
      ${name}
      SOURCE_DIR ${VENDOR_SOURCE_DIR_${name}}
      DOWNLOAD_COMMAND ""
      ${ARGV}
      GIT_SHALLOW TRUE
      GIT_COMMAND ""
      SYSTEM OVERRIDE_FIND_PACKAGE)
endmacro()
