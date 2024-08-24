#include <gtest/gtest.h>
#include <multipleint/mi.hpp>

TEST(Encoding, EmptyMultipleInt)
{
  constexpr auto mi = multipleint::multiple_int<7, std::uint8_t> {};

  EXPECT_EQ(0, mi.intv());
  EXPECT_EQ(0, mi.carry());
}

TEST(Encoding, NonTruncating)
{
  // array
  {
    constexpr auto l = multipleint::multiple_int<7, std::uint8_t>::encode<1>({0b01111111});

    EXPECT_EQ(0b01111111, l.intv());
    EXPECT_EQ(0, l.carry());
  }

  {
    constexpr auto l = multipleint::multiple_int<7, std::uint16_t>::encode<2>({0b01111111, 0b01110001});

    EXPECT_EQ(0b01110001'01111111, l.intv());
    EXPECT_EQ(0, l.carry());
  }

  {
    constexpr auto l =
        multipleint::multiple_int<7, std::uint32_t>::encode<4>({0b01111111, 0b01110001, 0b01111111, 0b01110001});

    EXPECT_EQ(0b01110001'01111111'01110001'01111111, l.intv());
    EXPECT_EQ(0, l.carry());
  }

  // single value
  {
    auto mi = multipleint::multiple_int<7, std::uint8_t> {};
    mi.encode<0>(0b01111111);

    EXPECT_EQ(0b01111111, mi.intv());
    EXPECT_EQ(0, mi.carry());
  }

  {
    auto mi = multipleint::multiple_int<7, std::uint16_t> {};
    mi.encode<0>(0b01111111);
    mi.encode<1>(0b01110001);

    EXPECT_EQ(0b01110001'01111111, mi.intv());
    EXPECT_EQ(0, mi.carry());
  }

  {
    auto mi = multipleint::multiple_int<7, std::uint32_t> {};
    mi.encode<0>(0b01111111);
    mi.encode<1>(0b01110001);
    mi.encode<2>(0b01111111);
    mi.encode<3>(0b01110001);

    EXPECT_EQ(0b01110001'01111111'01110001'01111111, mi.intv());
    EXPECT_EQ(0, mi.carry());
  }
}

TEST(Encoding, Truncating)
{
  // array
  {
    constexpr auto l = multipleint::multiple_int<7, std::uint8_t>::encode<1>({0b11111111});

    EXPECT_EQ(0b01111111, l.intv());
    EXPECT_EQ(0, l.carry());
  }

  {
    constexpr auto l = multipleint::multiple_int<7, std::uint16_t>::encode<2>({0b11111111, 0b11110001});

    EXPECT_EQ(0b01110001'01111111, l.intv());
    EXPECT_EQ(0, l.carry());
  }

  {
    constexpr auto l =
        multipleint::multiple_int<7, std::uint32_t>::encode<4>({0b11111111, 0b11110001, 0b11111111, 0b11110001});

    EXPECT_EQ(0b01110001'01111111'01110001'01111111, l.intv());
    EXPECT_EQ(0, l.carry());
  }

  // single value
  {
    auto mi = multipleint::multiple_int<7, std::uint8_t> {};
    mi.encode<0>(0b11111111);

    EXPECT_EQ(0b01111111, mi.intv());
    EXPECT_EQ(0, mi.carry());
  }

  {
    auto mi = multipleint::multiple_int<7, std::uint16_t> {};
    mi.encode<0>(0b11111111);
    mi.encode<1>(0b11110001);

    EXPECT_EQ(0b01110001'01111111, mi.intv());
    EXPECT_EQ(0, mi.carry());
  }

  {
    auto mi = multipleint::multiple_int<7, std::uint32_t> {};
    mi.encode<0>(0b11111111);
    mi.encode<1>(0b11110001);
    mi.encode<2>(0b11111111);
    mi.encode<3>(0b11110001);

    EXPECT_EQ(0b01110001'01111111'01110001'01111111, mi.intv());
    EXPECT_EQ(0, mi.carry());
  }
}
