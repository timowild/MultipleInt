# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(multipleintTests LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)

FetchContent_MakeAvailable(googletest)

# ---- Tests ----

add_executable(multipleint_test source/multipleint_test.cpp)
enable_warnings(multipleint_test)
add_subdirectory(source)
target_link_libraries(multipleint_test PRIVATE gtest_main)
target_compile_features(multipleint_test PRIVATE cxx_std_20)

target_include_directories(
  multipleint_test ${warning_guard} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

include(GoogleTest)
gtest_discover_tests(multipleint_test)
