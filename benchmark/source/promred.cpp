#include <numeric>
#include <vector>

#include <benchmark/benchmark.h>
#include <multipleint/mi.hpp>

#include "../include/bench_targets.hpp"
#include "../include/util.hpp"

template<typename SmallerInteger, typename BiggerInteger, typename ComparingInteger, std::size_t BitWidth>
static void sum_prom_int_bench(benchmark::State& state)
{
  constexpr std::size_t IntCount = multipleint::multiple_int<BitWidth, ComparingInteger>::IntCount;

  const auto n_elements = state.range(0);
  const Container<SmallerInteger> vals(n_elements * IntCount, SmallerInteger(1));

  const auto init = BiggerInteger {};
  for (auto _ : state) {
    benchmark::DoNotOptimize(bench::sum_red(exec_policy, vals.cbegin(), vals.cend(), init));
  }
}

template<typename SmallerInteger, typename BiggerInteger>
static void sum_prom_multi_int_bench(benchmark::State& state)
{
  auto xs = multipleint::detail::array_repeat<SmallerInteger::IntCount, int>(1);
  auto is = multipleint::detail::array_repeat<BiggerInteger::IntCount, int>(0);

  const auto n_elements = state.range(0);
  const Container<SmallerInteger> vals(n_elements, SmallerInteger::template encode<SmallerInteger::IntCount>(xs));
  const auto init = BiggerInteger::template encode<BiggerInteger::IntCount>(is);

  for (auto _ : state) {
    benchmark::DoNotOptimize(bench::sum_red(exec_policy, vals.cbegin(), vals.cend(), init).sum());
  }
}

// needs to be first defined benchmark!
BENCHMARK_TEMPLATE(sum_prom_int_bench, std::uint32_t, std::uint64_t, std::uint64_t, 31)->Name("_warmup_")->Arg(1 << 28);

// 64/32 bit
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(sum_prom_int_bench, std::uint32_t, std::uint64_t, std::uint64_t, 31)
    ->Name("promred-1-u32-u64x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_prom_multi_int_bench,
                   multipleint::multiple_int<15, std::uint32_t>,
                   multipleint::multiple_int<31, std::uint64_t>)
    ->Name("promred-1-mi<15-31, u32-u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_prom_int_bench, std::uint32_t, std::uint64_t, std::uint64_t, 19)
    ->Name("promred-1-u32-u64x3")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_prom_multi_int_bench,
                   multipleint::multiple_int<9, std::uint32_t>,
                   multipleint::multiple_int<19, std::uint64_t>)
    ->Name("promred-1-mi<9-19, u32-u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_prom_int_bench, std::uint32_t, std::uint64_t, std::uint64_t, 15)
    ->Name("promred-1-u32-u64x4")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_prom_multi_int_bench,
                   multipleint::multiple_int<7, std::uint32_t>,
                   multipleint::multiple_int<15, std::uint64_t>)
    ->Name("promred-1-mi<7-15, u32-u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//

// 32/16 bit
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(sum_prom_int_bench, std::uint16_t, std::uint32_t, std::uint32_t, 15)
    ->Name("promred-2-u16-u32x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_prom_multi_int_bench,
                   multipleint::multiple_int<7, std::uint16_t>,
                   multipleint::multiple_int<15, std::uint32_t>)
    ->Name("promred-2-mi<7-15, u16-u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_prom_int_bench, std::uint16_t, std::uint32_t, std::uint32_t, 9)
    ->Name("promred-2-u16-u32x3")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_prom_multi_int_bench,
                   multipleint::multiple_int<4, std::uint16_t>,
                   multipleint::multiple_int<9, std::uint32_t>)
    ->Name("promred-2-mi<4-9, u16-u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_prom_int_bench, std::uint16_t, std::uint32_t, std::uint32_t, 7)
    ->Name("promred-2-u16-u32x4")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_prom_multi_int_bench,
                   multipleint::multiple_int<3, std::uint16_t>,
                   multipleint::multiple_int<7, std::uint32_t>)
    ->Name("promred-2-mi<3-7, u16-u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//

// 16/8 bit
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(sum_prom_int_bench, std::uint8_t, std::uint16_t, std::uint16_t, 7)
    ->Name("promred-3-u8-u16x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_prom_multi_int_bench,
                   multipleint::multiple_int<3, std::uint8_t>,
                   multipleint::multiple_int<7, std::uint16_t>)
    ->Name("promred-3-mi<3-7, u8-u16>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_prom_int_bench, std::uint8_t, std::uint16_t, std::uint16_t, 3)
    ->Name("promred-3-u8-u16x4")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(sum_prom_multi_int_bench,
                   multipleint::multiple_int<1, std::uint8_t>,
                   multipleint::multiple_int<3, std::uint16_t>)
    ->Name("promred-3-mi<1-3, u8-u16>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//

BENCHMARK_MAIN();
