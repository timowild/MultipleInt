#include <gtest/gtest.h>
#include <multipleint/mi.hpp>

TEST(Traits, Masks)
{
  {
    using OneIntSixBit = multipleint::multiple_int<6, std::uint8_t>;
    EXPECT_EQ(0b00111111, OneIntSixBit::traits::int_mask);
    EXPECT_EQ(0b01000000, OneIntSixBit::traits::carry_mask);
    EXPECT_EQ(0b00100000, OneIntSixBit::traits::sign_mask);
    EXPECT_EQ(0b10000000, OneIntSixBit::traits::empty_mask);
    EXPECT_EQ(0b00000000,
              OneIntSixBit::traits::int_mask & OneIntSixBit::traits::carry_mask & OneIntSixBit::traits::empty_mask);
  }

  {
    using TwoInts3Bit = multipleint::multiple_int<3, std::uint8_t>;

    EXPECT_EQ(0b01110111, TwoInts3Bit::traits::int_mask);
    EXPECT_EQ(0b10001000, TwoInts3Bit::traits::carry_mask);
    EXPECT_EQ(0b01000100, TwoInts3Bit::traits::sign_mask);
    EXPECT_EQ(0b00000000, TwoInts3Bit::traits::empty_mask);
    EXPECT_EQ(0b00000000,
              TwoInts3Bit::traits::int_mask & TwoInts3Bit::traits::carry_mask & TwoInts3Bit::traits::empty_mask);
  }
}