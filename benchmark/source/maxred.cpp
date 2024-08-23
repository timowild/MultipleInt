#include <vector>

#include <benchmark/benchmark.h>
#include <multipleint/mi.hpp>

#include "../include/bench_targets.hpp"
#include "../include/util.hpp"

template<typename Integer,
         typename ComparingInteger,
         unsigned int BitWidth>
static void max_red_int_bench(benchmark::State& state)
{
  const auto n_elements = state.range(0);
  const Container<Integer> vals (n_elements * ((8 * sizeof(ComparingInteger)) / (BitWidth + 1)), Integer {1});

  for (auto _ : state) {
    benchmark::DoNotOptimize(bench::max_red(exec_policy, vals.cbegin(), vals.cend()));
  }
}

template<class T>
static void max_red_multi_int_bench(benchmark::State& state)
{
  auto xs = array_repeat<T::IntCount, int>(1);

  const auto n_elements = state.range(0);
  const Container<T> vals (n_elements, T::template encode<T::IntCount>(xs));

  for (auto _ : state) {
    benchmark::DoNotOptimize(bench::max_red(exec_policy, vals.cbegin(), vals.cend()).max());
  }
}

// needs to be first defined benchmark!
BENCHMARK_TEMPLATE(max_red_int_bench, std::uint32_t, std::uint32_t, 31)->Name("_warmup_")->Arg(1 << 28);

// 64 bit internal storage
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(max_red_int_bench, std::uint32_t, std::uint64_t, 31)
    ->Name("maxred-1-u32x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(max_red_multi_int_bench, multipleint::multiple_int<31, std::uint64_t>)
    ->Name("maxred-1-mi<31, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(max_red_int_bench, std::uint32_t, std::uint64_t, 16)
    ->Name("maxred-1-u32x3")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(max_red_multi_int_bench, multipleint::multiple_int<16, std::uint64_t>)
    ->Name("maxred-1-mi<16, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(max_red_int_bench, std::uint8_t, std::uint64_t, 7)
    ->Name("maxred-2-u8x8")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(max_red_multi_int_bench, multipleint::multiple_int<7, std::uint64_t>)
    ->Name("maxred-2-mi<7, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(max_red_int_bench, std::uint8_t, std::uint64_t, 2)
    ->Name("maxred-2-u8x21")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(max_red_multi_int_bench, multipleint::multiple_int<2, std::uint64_t>)
    ->Name("maxred-2-mi<2, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//

// 32 bit internal storage
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(max_red_int_bench, std::uint16_t, std::uint32_t, 15)
    ->Name("maxred-3-u16x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(max_red_multi_int_bench, multipleint::multiple_int<15, std::uint32_t>)
    ->Name("maxred-3-mi<15, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(max_red_int_bench, std::uint16_t, std::uint32_t, 8)
    ->Name("maxred-3-u16x3")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(max_red_multi_int_bench, multipleint::multiple_int<8, std::uint32_t>)
    ->Name("maxred-3-mi<8, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(max_red_int_bench, std::uint8_t, std::uint32_t, 7)
    ->Name("maxred-4-u8x4")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(max_red_multi_int_bench, multipleint::multiple_int<7, std::uint32_t>)
    ->Name("maxred-4-mi<7, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(max_red_int_bench, std::uint8_t, std::uint32_t, 2)
    ->Name("maxred-4-u8x10")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(max_red_multi_int_bench, multipleint::multiple_int<2, std::uint32_t>)
    ->Name("maxred-4-mi<2, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//

BENCHMARK_MAIN();
