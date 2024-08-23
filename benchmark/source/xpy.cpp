#include <vector>

#include <benchmark/benchmark.h>
#include <multipleint/mi.hpp>

#include "../include/bench_targets.hpp"
#include "../include/util.hpp"


template<typename Integer,
         typename ComparingInteger,
         unsigned int BitWidth>
static void xpy_int_bench(benchmark::State& state)
{
  const auto n_elements = state.range(0);
  const Container<Integer> x (n_elements * ((8 * sizeof(ComparingInteger)) / (BitWidth + 1)), Integer {1});

  // e.g. creating std::uint8_t, but multiple_int pendant calculates in std::uint32_t -> 4
  // more elements for same "size"
  Container<Integer> y(n_elements * ((8 * sizeof(ComparingInteger)) / (BitWidth + 1)), Integer {2});

  for (auto _ : state) {
    bench::xpy(exec_policy, x.cbegin(), x.cend(), y.cbegin(), y.begin());
  }
}

template<class T>
static void xpy_multi_int_bench(benchmark::State& state)
{
  auto xs = array_repeat<T::IntCount, int>(1);
  auto ys = array_repeat<T::IntCount, int>(2);

  const auto n_elements = state.range(0);
  const Container<T> x (n_elements, T::template encode<T::IntCount>(xs));
  Container<T> y(n_elements, T::template encode<T::IntCount>(ys));

  for (auto _ : state) {
    bench::xpy(exec_policy, x.cbegin(), x.cend(), y.cbegin(), y.begin());
  }
}

// needs to be first defined benchmark!
BENCHMARK_TEMPLATE(xpy_int_bench, std::uint32_t, std::uint32_t, 31)->Name("_warmup_")->Arg(1 << 28);

// 64 bit internal storage
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(xpy_int_bench, std::uint32_t, std::uint64_t, 31)
    ->Name("xpy-1-u32x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(xpy_multi_int_bench, multipleint::multiple_int<31, std::uint64_t>)
    ->Name("xpy-1-mi<31, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(xpy_int_bench, std::uint32_t, std::uint64_t, 16)
    ->Name("xpy-1-u32x3")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(xpy_multi_int_bench, multipleint::multiple_int<16, std::uint64_t>)
    ->Name("xpy-1-mi<16, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(xpy_int_bench, std::uint8_t, std::uint64_t, 7)
    ->Name("xpy-2-u8x8")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(xpy_multi_int_bench, multipleint::multiple_int<7, std::uint64_t>)
    ->Name("xpy-2-mi<7, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(xpy_int_bench, std::uint8_t, std::uint64_t, 1)
    ->Name("xpy-2-u8x32")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(xpy_multi_int_bench, multipleint::multiple_int<1, std::uint64_t>)
    ->Name("xpy-2-mi<1, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//

// 32 bit internal storage
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(xpy_int_bench, std::uint16_t, std::uint32_t, 15)
    ->Name("xpy-3-u16x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(xpy_multi_int_bench, multipleint::multiple_int<15, std::uint32_t>)
    ->Name("xpy-3-mi<15, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(xpy_int_bench, std::uint16_t, std::uint32_t, 8)
    ->Name("xpy-3-u16x3")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(xpy_multi_int_bench, multipleint::multiple_int<8, std::uint32_t>)
    ->Name("xpy-3-mi<8, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(xpy_int_bench, std::uint8_t, std::uint32_t, 7)
    ->Name("xpy-4-u8x4")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(xpy_multi_int_bench, multipleint::multiple_int<7, std::uint32_t>)
    ->Name("xpy-4-mi<7, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(xpy_int_bench, std::uint8_t, std::uint32_t, 1)
    ->Name("xpy-4-u8x16")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(xpy_multi_int_bench, multipleint::multiple_int<1, std::uint32_t>)
    ->Name("xpy-4-mi<1, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//

BENCHMARK_MAIN();
