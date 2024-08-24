#include <execution>
#include <vector>

#include <gtest/gtest.h>
#include <multipleint/mi.hpp>

#include "../../benchmark/include/bench_targets.hpp"
#include "../../benchmark/include/util.hpp"  // array_repeat

TEST(Sum_red_alt, TwoIntEightFullPositiveValues)
{
  {
    using m_int = multipleint::multiple_int<3, std::uint8_t>;

    constexpr auto l = m_int::encode<2>({2, 1});
    constexpr auto r = m_int::encode<2>({-2, 0});

    std::vector<m_int> v(2);
    int init = 0;

    v.push_back(l);
    v.push_back(r);

    auto result = bench::sum_red_alt(std::execution::par_unseq, v.begin(), v.end(), init);

    EXPECT_EQ(1, result);
  }
}

TEST(SumRed, Promotion)
{
  // 2 integers
  using SmallerInteger = multipleint::multiple_int<15, std::uint32_t>;
  using BiggerInteger = multipleint::multiple_int<31, std::uint64_t>;

  constexpr auto xs = array_repeat<SmallerInteger::IntCount, int>(2);
  constexpr auto is = array_repeat<BiggerInteger::IntCount, int>(0);

  // 10000 times
  const std::vector<SmallerInteger> vals(10000, SmallerInteger::encode<SmallerInteger::IntCount>(xs));
  constexpr auto init = BiggerInteger::encode<BiggerInteger::IntCount>(is);

  const auto expected = SmallerInteger::IntCount * vals.size() * 2;
  const auto s = bench::sum_red(std::execution::par_unseq, vals.cbegin(), vals.cend(), init).sum();

  EXPECT_EQ(expected, +s);
}