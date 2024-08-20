#include <gtest/gtest.h>
#include <multipleint/mi.hpp>

TEST(Encoding, NonTruncating)
{
  {
    constexpr auto l = multipleint::multiple_int<7, std::uint8_t>::encode<1>({0b01111111});

    EXPECT_EQ(0b01111111, l.intv());
    EXPECT_EQ(0, l.carry());
  }

  {
    constexpr auto l = multipleint::multiple_int<7, std::uint16_t>::encode<2>({0b01111111, 0b01110001});

    EXPECT_EQ(0b01111111'01110001, l.intv());
    EXPECT_EQ(0, l.carry());
  }

  {
    constexpr auto l =
        multipleint::multiple_int<7, std::uint32_t>::encode<4>({0b01111111, 0b01110001, 0b01111111, 0b01110001});

    EXPECT_EQ(0b01111111'01110001'01111111'01110001, l.intv());
    EXPECT_EQ(0, l.carry());
  }
}

TEST(Encoding, Truncating)
{
  {
    constexpr auto l = multipleint::multiple_int<7, std::uint8_t>::encode<1>({0b11111111});

    EXPECT_EQ(0b01111111, l.intv());
    EXPECT_EQ(0, l.carry());
  }

  {
    constexpr auto l = multipleint::multiple_int<7, std::uint16_t>::encode<2>({0b11111111, 0b11110001});

    EXPECT_EQ(0b01111111'01110001, l.intv());
    EXPECT_EQ(0, l.carry());
  }

  {
    constexpr auto l =
        multipleint::multiple_int<7, std::uint32_t>::encode<4>({0b11111111, 0b11110001, 0b11111111, 0b11110001});

    EXPECT_EQ(0b01111111'01110001'01111111'01110001, l.intv());
    EXPECT_EQ(0, l.carry());
  }
}
