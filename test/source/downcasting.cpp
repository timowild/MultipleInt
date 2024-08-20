#include <bitset>
#include <iostream>

#include <gtest/gtest.h>
#include <multipleint/mi.hpp>

// DownCast
TEST(DownCasting, Simple)
{
  {
    constexpr auto l = multipleint::multiple_int<7, std::uint16_t>::encode<2>({0b01111101, 0b0000111});

    constexpr auto t = static_cast<multipleint::multiple_int<3, std::uint8_t>>(l);

    EXPECT_EQ(0b0101'0111, t.intv()) << "Got " << std::bitset<8>(t.intv()) << "\n";
    EXPECT_EQ(0b1000'0000, t.carry());
  }

  {
    constexpr auto l = multipleint::multiple_int<15, std::uint32_t>::encode<2>({0b011101111101101, 0b001010100011000});

    constexpr auto t = static_cast<multipleint::multiple_int<7, std::uint16_t>>(l);

    EXPECT_EQ(0b01101101'00011000, t.intv()) << "Got " << std::bitset<16>(t.intv()) << "\n";
    EXPECT_EQ(0b10000000'00000000, t.carry());
  }
}

/* These tests use IntCount > 2 numbers, meaning the bits will be interleaved */
TEST(DownCasting, Interleaved)
{
  {
    constexpr auto l = multipleint::multiple_int<9, std::uint64_t>::encode<6>(
        {0b110011001, 0b001100000, 0b101010101, 0b111111111, 0b000000000, 0b100000001});

    // 11001
    // 00000
    // 10101
    // 11111
    // 00000
    // 00001

    // 0b00'11001'11111'00000'00000'10101'00001

    constexpr auto t = static_cast<multipleint::multiple_int<4, std::uint32_t>>(l);

    EXPECT_EQ(0b00'01001'01111'00000'00000'00101'00001, t.intv()) << "Got " << std::bitset<32>(t.intv()) << "\n";
    EXPECT_EQ(0b00'10000'10000'00000'00000'10000'00000, t.carry()) << "Got " << std::bitset<32>(t.carry()) << "\n";
  }
}