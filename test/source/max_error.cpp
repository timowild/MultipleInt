#include <gtest/gtest.h>
#include <multipleint/mi.hpp>
#include <multipleint/milimits.hpp>

TEST(ErrorMax, NormalValuesWithErrors)
{
  using target_type = multipleint::multiple_int<3, std::uint8_t>;
  using std::max;

  constexpr auto num = target_type::encode<2>({0b010, 0b010});

  constexpr auto numTimeThree = num + num + num;

  EXPECT_EQ(0b0110'0110, numTimeThree.intv());
  EXPECT_EQ(0b1000'1000, numTimeThree.carry());

  constexpr auto sec = target_type::encode<2>({0b001, 0b101});

  constexpr auto m = max(numTimeThree, sec);
  EXPECT_EQ(0b0110'0001, m.intv());
  EXPECT_EQ(0b1000'0000, m.carry());

  constexpr auto m2 = max(sec, numTimeThree);
  EXPECT_EQ(0b0110'0001, m2.intv());
  EXPECT_EQ(0b1000'0000, m2.carry());
}