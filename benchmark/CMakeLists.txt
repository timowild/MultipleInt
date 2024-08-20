# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(multipleintBenchmarks LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(googlebenchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.9.0.zip
)

FetchContent_MakeAvailable(googlebenchmark)

# ---- Benchmarks ----

set(BENCH_FILES 
    source/maxelem.cpp 
    source/maxred.cpp 
    source/promred.cpp 
    source/sumred.cpp 
    source/xpy.cpp
)

foreach(BENCH_FILE ${BENCH_FILES})
    get_filename_component(BENCH_NAME ${BENCH_FILE} NAME_WLE)
    add_executable(multipleint_${BENCH_NAME} ${BENCH_FILE})

    target_link_libraries(multipleint_${BENCH_NAME} PRIVATE 
        benchmark::benchmark
    )
    target_include_directories(multipleint_${BENCH_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)
    target_compile_features(multipleint_${BENCH_NAME} PRIVATE cxx_std_20)
    target_compile_options(multipleint_${BENCH_NAME} PRIVATE ${NVCXX_FLAGS})
    target_link_options(multipleint_${BENCH_NAME} PRIVATE ${NVCXX_FLAGS})
endforeach()
