cmake_minimum_required(VERSION 3.2)
project(Reversing)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Boost 1.64 COMPONENTS program_options system REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

include_directories($ENV{MXNET_HOME}/include)
include_directories($ENV{MXNET_HOME}/nnvm/include)
include_directories($ENV{MXNET_HOME}/dmlc-core/include)
include_directories($ENV{MXNET_HOME}/cpp-package/include)
link_directories($ENV{MXNET_HOME}/lib)

add_subdirectory(Reversing)