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

# Build tests.
file (GLOB_RECURSE AMSG_UNIT_TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file (GLOB_RECURSE AMSG_UNIT_TEST_FILES ${AMSG_UNIT_TEST_FILES} "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

add_executable (amsg_ut ${AMSG_UNIT_TEST_FILES})

if (AMSG_LINK_PROP)
  set_target_properties (amsg_ut PROPERTIES LINK_FLAGS "${AMSG_LINK_PROP}")
endif ()

if (AMSG_COMPILE_PROP)
  set_target_properties (amsg_ut PROPERTIES COMPILE_FLAGS "${AMSG_COMPILE_PROP}")
endif ()

install (TARGETS amsg_ut RUNTIME DESTINATION bin)
