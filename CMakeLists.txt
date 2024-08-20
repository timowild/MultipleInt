cmake_minimum_required(VERSION 3.18)

include(cmake/prelude.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works

project(
    multipleint
    VERSION 1.0.0
    DESCRIPTION "Store multiple small ints into one bigger int"
    LANGUAGES CXX
)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "NVHPC")
  set(NVCXX_FLAGS -stdpar=gpu -gpu=cc61,cc75,cuda11.7)
else()
  set(NVCXX_FLAGS "")
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

