cmake_minimum_required(VERSION 3.20)

include(cmake/prelude.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works

project(
    multipleint
    VERSION 1.0.0
    DESCRIPTION "Store multiple small ints into one bigger int"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_CUDA_COMPILER)
  enable_language(CUDA)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

include(cmake/variables.cmake)

# ---- Developer mode ----

if(multipleint_DEVELOPER_MODE)
  include(CTest)
  enable_testing()
  add_subdirectory(test)
  add_subdirectory(benchmark)
endif()

