#include <vector>

#include <benchmark/benchmark.h>
#include <multipleint/mi.hpp>

#include "../include/bench_targets.hpp"
#include "../include/util.hpp"

template<typename Integer, typename ComparingInteger, std::size_t BitWidth>
static void elemwise_max_int_bench(benchmark::State& state)
{
  constexpr std::size_t IntCount = multipleint::multiple_int<BitWidth, ComparingInteger>::IntCount;

  const auto n_elements = state.range(0);
  const Container<Integer> x(n_elements * IntCount, Integer {1});

  // e.g. creating std::uint8_t, but multiple_int pendant calculates in std::uint32_t -> 4
  // more elements for same "size"
  Container<Integer> y(n_elements * IntCount, Integer {2});

  for (auto _ : state) {
    bench::elemwise_max(exec_policy, x.cbegin(), x.cend(), y.cbegin(), y.begin());

    benchmark::DoNotOptimize(y);
  }
}

template<class T>
static void elemwise_max_multi_int_bench(benchmark::State& state)
{
  auto xs = multipleint::detail::array_repeat<T::IntCount, int>(1);
  auto ys = multipleint::detail::array_repeat<T::IntCount, int>(2);

  const auto n_elements = state.range(0);
  const Container<T> x(n_elements, T::template encode<T::IntCount>(xs));
  Container<T> y(n_elements, T::template encode<T::IntCount>(ys));

  for (auto _ : state) {
    bench::elemwise_max(exec_policy, x.cbegin(), x.cend(), y.cbegin(), y.begin());

    benchmark::DoNotOptimize(y);
  }
}

// needs to be first defined benchmark!
BENCHMARK_TEMPLATE(elemwise_max_int_bench, std::uint32_t, std::uint32_t, 31)->Name("_warmup_")->Arg(1 << 28);

// 64 bit internal storage
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(elemwise_max_int_bench, std::uint32_t, std::uint64_t, 31)
    ->Name("maxelem-1-u32x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(elemwise_max_multi_int_bench, multipleint::multiple_int<31, std::uint64_t>)
    ->Name("maxelem-1-mi<31, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(elemwise_max_int_bench, std::uint32_t, std::uint64_t, 16)
    ->Name("maxelem-1-u32x3")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(elemwise_max_multi_int_bench, multipleint::multiple_int<16, std::uint64_t>)
    ->Name("maxelem-1-mi<16, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(elemwise_max_int_bench, std::uint8_t, std::uint64_t, 7)
    ->Name("maxelem-2-u8x8")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(elemwise_max_multi_int_bench, multipleint::multiple_int<7, std::uint64_t>)
    ->Name("maxelem-2-mi<7, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(elemwise_max_int_bench, std::uint8_t, std::uint64_t, 2)
    ->Name("maxelem-2-u8x21")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(elemwise_max_multi_int_bench, multipleint::multiple_int<2, std::uint64_t>)
    ->Name("maxelem-2-mi<2, u64>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//

// 32 bit internal storage
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(elemwise_max_int_bench, std::uint16_t, std::uint32_t, 15)
    ->Name("maxelem-3-u16x2")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(elemwise_max_multi_int_bench, multipleint::multiple_int<15, std::uint32_t>)
    ->Name("maxelem-3-mi<15, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(elemwise_max_int_bench, std::uint16_t, std::uint32_t, 8)
    ->Name("maxelem-3-u16x3")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(elemwise_max_multi_int_bench, multipleint::multiple_int<8, std::uint32_t>)
    ->Name("maxelem-3-mi<8, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//

BENCHMARK_TEMPLATE(elemwise_max_int_bench, std::uint8_t, std::uint32_t, 7)
    ->Name("maxelem-4-u8x4")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(elemwise_max_multi_int_bench, multipleint::multiple_int<7, std::uint32_t>)
    ->Name("maxelem-4-mi<7, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(elemwise_max_int_bench, std::uint8_t, std::uint32_t, 2)
    ->Name("maxelem-4-u8x10")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

BENCHMARK_TEMPLATE(elemwise_max_multi_int_bench, multipleint::multiple_int<2, std::uint32_t>)
    ->Name("maxelem-4-mi<2, u32>")
    ->RangeMultiplier(1 << 2)
    ->Range(1 << 14, 1 << 30);

//--------------------------------------------------------------------------------------------//

BENCHMARK_MAIN();
