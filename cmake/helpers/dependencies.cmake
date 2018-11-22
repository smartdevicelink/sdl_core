include_guard(GLOBAL)

set(dependency_descriptor_dir "${CMAKE_CURRENT_LIST_DIR}/dependencies")
file(GLOB dependency_descriptors "${dependency_descriptor_dir}/*")
foreach(dependency IN LISTS dependency_descriptors)
  get_filename_component(dependency_name "${dependency}" NAME_WE)
  list(APPEND buildable_dependencies "${dependency_name}")
endforeach()

function(SDL_find_package)
  # Handle REQUIRED somehow so this doesn't fail
  if(REQUIRED IN_LIST ARGV)
    set(${ARGV0}_REQUIRED ON)
  endif()
  list(REMOVE_ITEM ARGV REQUIRED)

  find_package(${ARGV} QUIET)
  if(NOT ${ARGV0}_FOUND
     AND NOT
         ${ARGV0}
         IN_LIST
         buildable_dependencies)
    if(${ARGV0}_REQUIRED)
      message(
        FATAL_ERROR "Required package ${ARGV0} was not found and there is no "
                    "dependency descriptor file which can provide it.")
    endif()
  else()
    include("${dependency_descriptor_dir}/${ARGV0}.cmake")
  endif()
  if(${ARGV0}_REQUIRED AND NOT ${ARGV0}_FOUND)
    message(
      FATAL_ERROR
        "Required package ${ARGV0} was missing and the dependency "
        "descriptor ${dependency_descriptor_dir}/${ARGV0}.cmake did not provide it."
      )
  endif()
endfunction()
