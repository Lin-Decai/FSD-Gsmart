# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_skidpad_icp_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED skidpad_icp_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(skidpad_icp_FOUND FALSE)
  elseif(NOT skidpad_icp_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(skidpad_icp_FOUND FALSE)
  endif()
  return()
endif()
set(_skidpad_icp_CONFIG_INCLUDED TRUE)

# output package information
if(NOT skidpad_icp_FIND_QUIETLY)
  message(STATUS "Found skidpad_icp: 1.0.0 (${skidpad_icp_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'skidpad_icp' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${skidpad_icp_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(skidpad_icp_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${skidpad_icp_DIR}/${_extra}")
endforeach()
