#
# This file is part of the CMake build system for Amsg
#
# CMake auto-generated configuration options.
# Do not check in modified versions of this file.
#
# Copyright (c) 2012 Ning Ding(lordoffox£¬lordoffox@gmail.com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# See https://github.com/lordoffox/amsg for latest version.
#

# Add example macro.
macro (amsg_add_example target_name)
  file (GLOB_RECURSE AMSG_EXAMPLE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
  file (GLOB_RECURSE AMSG_EXAMPLE_FILES ${AMSG_EXAMPLE_FILES} "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
  add_executable (${target_name} ${AMSG_EXAMPLE_FILES})

  if (AMSG_LINK_PROP)
    set_target_properties (${target_name} PROPERTIES LINK_FLAGS "${AMSG_LINK_PROP}")
  endif ()

  if (AMSG_COMPILE_PROP)
    set_target_properties (${target_name} PROPERTIES COMPILE_FLAGS "${AMSG_COMPILE_PROP}")
  endif ()

  if (EXAMPLES_LINK_LIBS)
    target_link_libraries (${target_name} ${EXAMPLES_LINK_LIBS})
  endif ()
  install (TARGETS ${target_name} RUNTIME DESTINATION bin)
endmacro (amsg_add_example)

# size_of
add_subdirectory (size_of)

# smax
add_subdirectory (smax)

# sfix
add_subdirectory (sfix)
