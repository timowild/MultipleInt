#include <vector>

#include <benchmark/benchmark.h>
#include <multipleint/mi.hpp>

#include "../include/bench_targets.hpp"
#include "../include/util.hpp"

template<typename Integer, typename ComparingInteger, std::size_t BitWidth>
static void sum_red_int_bench(benchmark::State& state)
{
  constexpr std::size_t IntCount = multipleint::multiple_int<BitWidth, ComparingInteger>::IntCount;

  const auto n_elements = state.range(0);
  const Container<Integer> vals(n_elements * IntCount, Integer {1});

  const auto init = Integer {};

  for (auto _ : state) {
    benchmark::DoNotOptimize(bench::sum_red(exec_policy, vals.cbegin(), vals.cend(), init));
  }
}

template<class T>
static void sum_red_multi_int_bench(benchmark::State& state)
{
  auto xs = array_repeat<T::IntCount, int>(1);
  auto is = array_repeat<T::IntCount, int>(0);

  const auto n_elements = state.range(0);
  const Container<T> vals(n_elements, T::template encode<T::IntCount>(xs));
  const auto init = T::template encode<T::IntCount>(is);

  for (auto _ : state) {
    benchmark::DoNotOptimize(bench::sum_red(exec_policy, vals.cbegin(), vals.cend(), init).sum());
  }
}

// needs to be first defined benchmark!
BENCHMARK_TEMPLATE(sum_red_int_bench, std::uint32_t, std::uint32_t, 31)->Name("_warmup_")->Arg(1 << 28);

// 64 bit internal storage
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(sum_red_int_bench, std::uint32_t, std::uint64_t, 31)
    ->Name("sumred-1-u32x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_red_multi_int_bench, multipleint::multiple_int<31, std::uint64_t>)
    ->Name("sumred-1-mi<31, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_red_int_bench, std::uint32_t, std::uint64_t, 16)
    ->Name("sumred-1-u32x3")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_red_multi_int_bench, multipleint::multiple_int<16, std::uint64_t>)
    ->Name("sumred-1-mi<16, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(sum_red_int_bench, std::uint8_t, std::uint64_t, 7)
    ->Name("sumred-2-u8x8")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_red_multi_int_bench, multipleint::multiple_int<7, std::uint64_t>)
    ->Name("sumred-2-mi<7, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_red_int_bench, std::uint8_t, std::uint64_t, 1)
    ->Name("sumred-2-u8x32")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_red_multi_int_bench, multipleint::multiple_int<1, std::uint64_t>)
    ->Name("sumred-2-mi<1, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//

// 32 bit internal storage
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(sum_red_int_bench, std::uint16_t, std::uint32_t, 15)
    ->Name("sumred-3-u16x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_red_multi_int_bench, multipleint::multiple_int<15, std::uint32_t>)
    ->Name("sumred-3-mi<15, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_red_int_bench, std::uint16_t, std::uint32_t, 8)
    ->Name("sumred-3-u16x3")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_red_multi_int_bench, multipleint::multiple_int<8, std::uint32_t>)
    ->Name("sumred-3-mi<8, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(sum_red_int_bench, std::uint8_t, std::uint32_t, 7)
    ->Name("sumred-4-u8x4")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_red_multi_int_bench, multipleint::multiple_int<7, std::uint32_t>)
    ->Name("sumred-4-mi<7, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_red_int_bench, std::uint8_t, std::uint32_t, 1)
    ->Name("sumred-4-u8x16")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_red_multi_int_bench, multipleint::multiple_int<1, std::uint32_t>)
    ->Name("sumred-4-mi<1, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//

BENCHMARK_MAIN();
