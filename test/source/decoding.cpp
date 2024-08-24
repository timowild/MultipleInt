#include <gtest/gtest.h>
#include <multipleint/mi.hpp>

TEST(Decoding, FromEncoding)
{
  {
    constexpr auto l = multipleint::multiple_int<3, std::uint8_t>::encode<2>({2, 1});

    EXPECT_EQ(2, l.extract<0>());
    EXPECT_EQ(1, l.extract<1>());

    EXPECT_EQ(2, l.decode<2>()[0]);
    EXPECT_EQ(1, l.decode<2>()[1]);
  }

  {
    constexpr auto l = multipleint::multiple_int<3, std::uint16_t>::encode<3>({2, 1, -2});

    EXPECT_EQ(2, l.extract<0>());
    EXPECT_EQ(1, l.extract<1>());
    EXPECT_EQ(-2, l.extract<2>());

    EXPECT_EQ(2, l.decode<3>()[0]);
    EXPECT_EQ(1, l.decode<3>()[1]);
    EXPECT_EQ(-2, l.decode<3>()[2]);
  }

  {
    constexpr auto l = multipleint::multiple_int<7, std::uint32_t>::encode<2>({63, 23});

    EXPECT_EQ(63, l.extract<0>());
    EXPECT_EQ(23, l.extract<1>());

    EXPECT_EQ(63, l.decode<2>()[0]);
    EXPECT_EQ(23, l.decode<2>()[1]);
  }

  {
    // 'Sign bit' is set -> Decoded value will be negative
    constexpr auto l = multipleint::multiple_int<7, std::uint32_t>::encode<1>(std::array {64});

    EXPECT_EQ(-64, l.extract<0>());
    EXPECT_EQ(-64, l.decode<1>()[0]);
  }
}

TEST(Decoding, SignedAddition)
{
  {
    constexpr auto l = multipleint::multiple_int<3, std::uint8_t>::encode<2>({2, 1});
    constexpr auto r = multipleint::multiple_int<3, std::uint8_t>::encode<2>({-2, 0});

    constexpr auto s = l + r;

    EXPECT_EQ(0, s.extract<0>());
    EXPECT_EQ(1, s.extract<1>());

    EXPECT_EQ(0, s.decode<2>()[0]);
    EXPECT_EQ(1, s.decode<2>()[1]);
  }

  {
    constexpr auto l = multipleint::multiple_int<3, std::uint16_t>::encode<3>({2, 1, -2});
    constexpr auto r = multipleint::multiple_int<3, std::uint16_t>::encode<3>({-2, 1, -2});

    constexpr auto s = l + r;

    EXPECT_EQ(0, s.extract<0>());
    EXPECT_EQ(2, s.extract<1>());
    EXPECT_EQ(-4, s.extract<2>());

    EXPECT_EQ(0, s.decode<3>()[0]);
    EXPECT_EQ(2, s.decode<3>()[1]);
    EXPECT_EQ(-4, s.decode<3>()[2]);
  }

  {
    constexpr auto l = multipleint::multiple_int<7, std::uint32_t>::encode<2>({63, 23});
    constexpr auto r = multipleint::multiple_int<7, std::uint32_t>::encode<2>({-1, 15});

    constexpr auto s = l + r;

    EXPECT_EQ(62, s.extract<0>());
    EXPECT_EQ(38, s.extract<1>());

    EXPECT_EQ(62, s.decode<2>()[0]);
    EXPECT_EQ(38, s.decode<2>()[1]);
  }
}