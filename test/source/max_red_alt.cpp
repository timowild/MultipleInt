#include <execution>

#include <gtest/gtest.h>
#include <multipleint/mi.hpp>

#include "../../benchmark/include/bench_targets.hpp"

TEST(Max_red_alt, FourIntSixteenFull_PositiveValues)
{
  using m_int = multipleint::multiple_int<3, std::uint16_t>;

  constexpr auto a = m_int::encode<4>({0b010, 0b001, 0b000, 0b001});
  constexpr auto b = m_int::encode<4>({0b010, 0b011, 0b001, 0b001});
  constexpr auto c = m_int::encode<4>({0b010, 0b000, 0b000, 0b000});
  constexpr auto d = m_int::encode<4>({0b010, 0b000, 0b001, 0b011});

  std::vector<m_int> v({a, b, c, d});
  constexpr auto init = std::numeric_limits<std::int16_t>::lowest();

  auto result = bench::max_red_alt(std::execution::par_unseq, v.begin(), v.end(), init);

  EXPECT_EQ(0b0011, result);
}

TEST(Max_red_alt, TwoIntSixteenFull_NegativeValues)
{
  using m_int = multipleint::multiple_int<3, std::uint16_t>;

  constexpr auto a = m_int::encode<4>({0b110, 0b101, 0b100, 0b101});
  constexpr auto b = m_int::encode<4>({0b100, 0b111, 0b101, 0b101});
  constexpr auto c = m_int::encode<4>({0b101, 0b100, 0b100, 0b100});
  constexpr auto d = m_int::encode<4>({0b110, 0b100, 0b101, 0b111});

  std::vector<m_int> v({a, b, c, d});
  constexpr auto init = std::numeric_limits<std::int16_t>::lowest();

  auto result = bench::max_red_alt(std::execution::par_unseq, v.begin(), v.end(), init);

  EXPECT_EQ(-1, result);
}

TEST(Max_red_alt, TwoIntSixteenFull_PositiveAndNegativeValues)
{
  using m_int = multipleint::multiple_int<3, std::uint16_t>;

  constexpr auto a = m_int::encode<4>({0b110, 0b101, 0b000, 0b101});
  constexpr auto b = m_int::encode<4>({0b010, 0b111, 0b001, 0b101});
  constexpr auto c = m_int::encode<4>({0b110, 0b100, 0b100, 0b100});
  constexpr auto d = m_int::encode<4>({0b010, 0b100, 0b101, 0b010});

  std::vector<m_int> v({a, b, c, d});
  constexpr auto init = std::numeric_limits<std::int16_t>::lowest();

  auto result = bench::max_red_alt(std::execution::par_unseq, v.begin(), v.end(), init);

  EXPECT_EQ(0b0010, result);
}