# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/beanie/Documents/beangine/external/SDL3_image-3.4.8")
  file(MAKE_DIRECTORY "/home/beanie/Documents/beangine/external/SDL3_image-3.4.8")
endif()
file(MAKE_DIRECTORY
  "/home/beanie/Documents/beangine/build/_deps/sdl_image-build"
  "/home/beanie/Documents/beangine/build/_deps/sdl_image-subbuild/sdl_image-populate-prefix"
  "/home/beanie/Documents/beangine/build/_deps/sdl_image-subbuild/sdl_image-populate-prefix/tmp"
  "/home/beanie/Documents/beangine/build/_deps/sdl_image-subbuild/sdl_image-populate-prefix/src/sdl_image-populate-stamp"
  "/home/beanie/Documents/beangine/build/_deps/sdl_image-subbuild/sdl_image-populate-prefix/src"
  "/home/beanie/Documents/beangine/build/_deps/sdl_image-subbuild/sdl_image-populate-prefix/src/sdl_image-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/beanie/Documents/beangine/build/_deps/sdl_image-subbuild/sdl_image-populate-prefix/src/sdl_image-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/beanie/Documents/beangine/build/_deps/sdl_image-subbuild/sdl_image-populate-prefix/src/sdl_image-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
