#include <gtest/gtest.h>
#include <multipleint/milimits.hpp>

TEST(Limits, Min)
{
  using value_type = multipleint::multiple_int<4, std::uint16_t>;

  constexpr auto min = std::numeric_limits<value_type>::min();

  EXPECT_EQ(min.intv(), 0b0'01000'01000'01000);
  EXPECT_EQ(min.carry(), 0);
}

TEST(Limits, Max)
{
  using value_type = multipleint::multiple_int<4, std::uint16_t>;

  constexpr auto max = std::numeric_limits<value_type>::max();

  EXPECT_EQ(max.intv(), 0b0'00111'00111'00111);
  EXPECT_EQ(max.carry(), 0);
}