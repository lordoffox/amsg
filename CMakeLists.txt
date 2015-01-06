#
# This file is part of the CMake build system for Amsg
#
# CMake auto-generated configuration options.
# Do not check in modified versions of this file.
#
# Copyright (c) 2012 Ning Ding(lordoffox，lordoffox@gmail.com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# See https://github.com/lordoffox/amsg for latest version.
#

cmake_minimum_required (VERSION 2.8.6 FATAL_ERROR)
project (amsg)

# The version number.
set (AMSG_VERSION_MAJOR 1)
set (AMSG_VERSION_MINOR 0)

# Provide user options to customise the build process.
option (AMSG_BUILD_EXAMPLE "Build Amsg examples" ON)
option (AMSG_BUILD_TEST "Build Amsg tests" ON)
option (AMSG_STD_CXX11 "Build Amsg using C++11" OFF)

if (AMSG_BUILD_TEST OR AMSG_BUILD_EXAMPLE)
  if (UNIX)
    option (AMSG_STATIC "AMSG test and example runtime static" OFF)
  endif ()

  if (AMSG_STD_CXX11)
    add_definitions (-DAMSG_STD_CXX11)
  endif ()

  if (WIN32)
    set (AMSG_WINVER "0x0501" CACHE STRING "Windows version maro. Default is 0x0501 - winxp, user can reset")
    add_definitions (-D_WIN32_WINNT=${AMSG_WINVER})
  endif ()

  # Add the source and build tree to the search path for include gce header files.
  include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../)

  # Set glibc for static runtime.
  if (AMSG_STATIC)
    set (GLIBC_INCLUDEDIR "" CACHE PATH "Path to glibc include directory")
    set (GLIBC_LIBRARYDIR "" CACHE PATH "Path to glibc libraries directory")
    if (GLIBC_INCLUDEDIR)
      include_directories (${GLIBC_INCLUDEDIR})
    endif ()
    if (GLIBC_LIBRARYDIR)
      link_directories (${GLIBC_LIBRARYDIR})
    endif ()
  endif ()

  # Boost headers only search.
  find_package (Boost 1.55.0)
  if (Boost_FOUND)
    include_directories (${Boost_INCLUDE_DIRS})
  else ()
    message (FATAL_ERROR "Boost not found! Please set BOOST_ROOT and rerun cmake.")
  endif ()

  set (CMAKE_VERBOSE_MAKEFILE true)

  if (AMSG_STATIC)
    set (AMSG_LINK_PROP "-static -static-libgcc -static-libstdc++")
  endif ()

  if (AMSG_STD_CXX11 AND NOT WIN32)
    set (AMSG_COMPILE_PROP "-std=c++11")
    if (APPLE)
      set (AMSG_COMPILE_PROP "${AMSG_COMPILE_PROP} -stdlib=libc++")
    endif ()
  endif ()

  if (WIN32)
    if (${CMAKE_GENERATOR} MATCHES "Visual Studio 11 *" OR ${CMAKE_GENERATOR} MATCHES "Visual Studio 12 *")
      set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
    endif ()
  endif ()
  
  # Build examples.
  if (AMSG_BUILD_EXAMPLE)
    add_subdirectory (example)
  endif ()

  # Build tests.
  if (AMSG_BUILD_TEST)
    add_subdirectory (test)
  endif ()
endif ()

file (GLOB AMSG_HEADER_FILES "${PROJECT_SOURCE_DIR}/*.hpp")
install (
  FILES ${AMSG_HEADER_FILES} DESTINATION include/amsg
  )

# Build a CPack driven installer package.
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${AMSG_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${AMSG_VERSION_MINOR}")
set (CPACK_PACKAGE_CONTACT "Ning Ding: lordoffox@gmail.com")
include (CPack)
