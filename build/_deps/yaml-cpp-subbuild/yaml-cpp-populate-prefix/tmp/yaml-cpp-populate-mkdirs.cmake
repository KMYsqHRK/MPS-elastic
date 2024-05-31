# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/l/MPS-elastic/build/_deps/yaml-cpp-src"
  "/home/l/MPS-elastic/build/_deps/yaml-cpp-build"
  "/home/l/MPS-elastic/build/_deps/yaml-cpp-subbuild/yaml-cpp-populate-prefix"
  "/home/l/MPS-elastic/build/_deps/yaml-cpp-subbuild/yaml-cpp-populate-prefix/tmp"
  "/home/l/MPS-elastic/build/_deps/yaml-cpp-subbuild/yaml-cpp-populate-prefix/src/yaml-cpp-populate-stamp"
  "/home/l/MPS-elastic/build/_deps/yaml-cpp-subbuild/yaml-cpp-populate-prefix/src"
  "/home/l/MPS-elastic/build/_deps/yaml-cpp-subbuild/yaml-cpp-populate-prefix/src/yaml-cpp-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/l/MPS-elastic/build/_deps/yaml-cpp-subbuild/yaml-cpp-populate-prefix/src/yaml-cpp-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/l/MPS-elastic/build/_deps/yaml-cpp-subbuild/yaml-cpp-populate-prefix/src/yaml-cpp-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
