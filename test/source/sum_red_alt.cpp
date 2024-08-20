#include <execution>

#include <gtest/gtest.h>
#include <multipleint/mi.hpp>

#include "../../benchmark/include/bench_targets.hpp"

TEST(Sum_red_alt, TwoIntEightFull_PositiveValues)
{
  using m_int = multipleint::multiple_int<3, std::uint8_t>;

  constexpr auto l = m_int::encode<2>({2, 1});
  constexpr auto r = m_int::encode<2>({2, 0});

  std::vector<m_int> v({l, r});
  int init = 0;

  auto result = bench::sum_red_alt(std::execution::par_unseq, v.begin(), v.end(), init);

  EXPECT_EQ(5, result);
}

TEST(Sum_red_alt, TwoIntEightFull_NegativeValues)
{
  using m_int = multipleint::multiple_int<3, std::uint8_t>;

  constexpr auto l = m_int::encode<2>({-2, -1});
  constexpr auto r = m_int::encode<2>({-2, 0});

  std::vector<m_int> v({l, r});
  int init = 0;

  auto result = bench::sum_red_alt(std::execution::par_unseq, v.begin(), v.end(), init);

  EXPECT_EQ(-5, result);
}

TEST(Sum_red_alt, TwoIntEightFull_PositiveAndNegativeValues)
{
  using m_int = multipleint::multiple_int<3, std::uint8_t>;

  constexpr auto l = m_int::encode<2>({2, 1});
  constexpr auto r = m_int::encode<2>({-2, 0});

  std::vector<m_int> v({l, r});
  int init = 0;

  auto result = bench::sum_red_alt(std::execution::par_unseq, v.begin(), v.end(), init);

  EXPECT_EQ(1, result);
}
