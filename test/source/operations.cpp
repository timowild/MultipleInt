#include <bitset>

#include <gtest/gtest.h>
#include <multipleint/mi.hpp>
#include <multipleint/milimits.hpp>

using std::max;

TEST(UnaryMinus, InvertPosToNeg)
{
  using target_type = multipleint::multiple_int<15, std::uint64_t>;

  constexpr auto num = target_type::encode<4>({0x355D, 0x7FFF, 0x0000, 0x3FFF});
  constexpr auto res = -num;
  constexpr auto expected = target_type::encode<4>({0x4AA3, 0x0001, 0x0000, 0x4001});

  EXPECT_EQ(expected.intv(), res.intv()) << "Expected " << std::hex << expected.intv() << ", got " << res.intv()
                                         << "\n";
  EXPECT_EQ(expected.carry(), res.carry())
      << "Expected " << std::hex << expected.carry() << ", got " << res.carry() << "\n";
}

TEST(UnaryMinus, SanityCheck)
{
  using target_type = multipleint::multiple_int<7, std::uint8_t>;

  constexpr auto num = target_type::encode<1>({0x7A});  // 0111'1010 -> 00000101 + 1
  constexpr auto res = -num;

  EXPECT_EQ(0b00000110, res.intv()) << "Expected " << std::hex << 0b00000110 << ", got " << res.intv() << "\n";
  EXPECT_EQ(0b00000000, res.carry()) << "Expected " << std::hex << 0b10000000 << ", got " << res.intv() << "\n";
}

TEST(UnaryMinus, InvertNegToPos)
{
  using target_type = multipleint::multiple_int<15, std::uint64_t>;

  constexpr auto num = target_type::encode<4>({0x4AA3, 0x0001, 0x0000, 0x4001});
  constexpr auto res = -num;
  constexpr auto expected = target_type::encode<4>({0x355D, 0x7FFF, 0x0000, 0x3FFF});

  EXPECT_EQ(expected.intv(), res.intv()) << "Expected " << std::hex << expected.intv() << ", got " << res.intv()
                                         << "\n";
  EXPECT_EQ(expected.carry(), res.carry())
      << "Expected " << std::hex << expected.carry() << ", got " << res.carry() << "\n";
}

TEST(UnaryMinus, Zero)
{
  using target_type = multipleint::multiple_int<3, std::uint8_t>;

  constexpr auto num = target_type::encode<2>({0, 0});
  constexpr auto res = -num;
  constexpr auto expected = target_type::encode<2>({0, 0});

  EXPECT_EQ(expected.intv(), res.intv());
  EXPECT_EQ(expected.carry(), res.carry());
}

TEST(UnaryMinus, NonsymmetricalTwosComp)
{
  {
    using target_type = multipleint::multiple_int<3, std::uint8_t>;

    constexpr auto num = target_type::encode<2>({0b100, 0b100});
    constexpr auto res = -num;
    constexpr auto expected = target_type::encode<2>({0b100, 0b100});

    // identical values, but set carry bits
    EXPECT_EQ(expected.intv(), res.intv())
        << "Expected " << std::hex << expected.intv() << ", got " << res.intv() << "\n";
    EXPECT_EQ(0b1000'1000, res.carry());
  }
  {
    using target_type = multipleint::multiple_int<2, std::uint16_t>;

    constexpr auto num = target_type::encode<5>({0b10, 0b10, 0b10, 0b10, 0b10});
    constexpr auto res = -num;
    constexpr auto expected = target_type::encode<5>({0b10, 0b10, 0b10, 0b10, 0b10});

    // identical values, but set carry bits
    EXPECT_EQ(expected.intv(), res.intv())
        << "Expected " << std::hex << expected.intv() << ", got " << res.intv() << "\n";
    EXPECT_EQ(0b100'100'100'100'100, res.carry());
  }
}

TEST(UnaryMinus, Examples)
{
  using target_type = multipleint::multiple_int<15, std::uint64_t>;

  constexpr auto num = target_type::encode<4>({-5, +2, -20, +10});
  constexpr auto res = -num;
  constexpr auto expected = target_type::encode<4>({+5, -2, +20, -10});

  EXPECT_EQ(expected.intv(), res.intv()) << "Expected " << std::hex << expected.intv() << ", got " << res.intv()
                                         << "\n";

  EXPECT_EQ(expected.carry(), res.carry())
      << "Expected " << std::hex << expected.carry() << ", got " << res.carry() << "\n";
}

TEST(BinaryMinus, PosMinusPos)
{
  using target_type = multipleint::multiple_int<15, std::uint64_t>;

  constexpr auto l = target_type::encode<4>({0x3FFF, 0x0001, 0x0000, 0x1047});
  constexpr auto r = target_type::encode<4>({0x3C7F, 0x0002, 0x0000, 0x1FE7});
  constexpr auto res = l - r;
  constexpr auto expected = target_type::encode<4>({0x0380, 0x7FFF, 0x0000, 0x7060});

  EXPECT_EQ(expected.intv(), res.intv()) << "Expected " << std::hex << expected.intv() << ", got " << res.intv()
                                         << "\n";

  EXPECT_EQ(expected.carry(), res.carry())
      << "Expected " << std::hex << expected.carry() << ", got " << res.carry() << "\n";
}

TEST(BinaryMinus, MinusMinusMinus)
{
  using target_type = multipleint::multiple_int<15, std::uint64_t>;

  constexpr auto l = target_type::encode<4>({0x75F7, 0x7FFF, 0x5FFF, 0x7C86});
  constexpr auto r = target_type::encode<4>({0x7C86, 0x7FFF, 0x7FFF, 0x75F7});
  constexpr auto res = l - r;
  constexpr auto expected = target_type::encode<4>({0x7971, 0x0000, 0x6000, 0x068F});

  EXPECT_EQ(expected.intv(), res.intv()) << "Expected " << std::hex << expected.intv() << ", got " << res.intv()
                                         << "\n";
  EXPECT_EQ(expected.carry(), res.carry())
      << "Expected " << std::hex << expected.carry() << ", got " << res.carry() << "\n";
}

TEST(Subtraction, NoOverflow)
{
  using target_type = multipleint::multiple_int<3, std::uint8_t>;

  // no carries in LHS or RHS, no min error bit
  {
    constexpr auto l = target_type::encode<2>({2, 1});
    constexpr auto r = target_type::encode<2>({1, -1});
    constexpr auto out = l - r;
    constexpr auto expected_int = 0b0'010'0'001;
    constexpr auto expected_carry = 0b0;

    EXPECT_EQ(expected_int, out.intv());
    EXPECT_EQ(expected_carry, out.carry());
  }

  // no carries in LHS or RHS, min error bit
  {
    constexpr auto l = target_type::encode<2>({-3, -3});
    constexpr auto r = target_type::encode<2>({-4, -4});
    constexpr auto out = l - r;
    constexpr auto expected_int = 0b0'001'0'001;
    constexpr auto expected_carry = 0b0;

    EXPECT_EQ(expected_int, out.intv());
    EXPECT_EQ(expected_carry, out.carry());
  }

  // no carries in LHS, carry in RHS, no min error bit
  {
    constexpr auto l = target_type::encode<2>({-3, -3});
    constexpr auto r = target_type::encode<2>({1, 3}) + target_type::encode<2>({1, 3});

    EXPECT_EQ(0b0110'0010, r.intv());
    EXPECT_EQ(0b1000'0000, r.carry());

    constexpr auto out = l - r;
    constexpr auto expected_int = 0b0'111'0'011;
    constexpr auto expected_carry = 0b1000'1000;

    EXPECT_EQ(expected_int, out.intv());
    EXPECT_EQ(expected_carry, out.carry());
  }

  // no carries in LHS, set carries in RHS, min error bits
  {
    constexpr auto l = target_type::encode<2>({-4, -4});
    constexpr auto r = target_type::encode<2>({-2, 3}) + target_type::encode<2>({-2, 3});

    EXPECT_EQ(0b0110'0100, r.intv());
    EXPECT_EQ(0b1000'0000, r.carry());

    constexpr auto out = l - r;

    constexpr auto expected_int = 0b0'110'0'000;
    constexpr auto expected_carry = 0b1000'0000;

    EXPECT_EQ(expected_int, out.intv());
    EXPECT_EQ(expected_carry, out.carry());
  }

  // carry in LHS, none in RHS, no min error bit
  {
    constexpr auto l = target_type::encode<2>({1, 3}) + target_type::encode<2>({1, 3});

    EXPECT_EQ(0b0110'0010, l.intv());
    EXPECT_EQ(0b1000'0000, l.carry());

    constexpr auto r = target_type::encode<2>({1, -1});
    constexpr auto out = l - r;
    constexpr auto expected_int = 0b0'111'0'001;
    constexpr auto expected_carry = 0b1000'0000;

    EXPECT_EQ(expected_int, out.intv());
    EXPECT_EQ(expected_carry, out.carry());
  }

  // carry in LHS, none in RHS, min error bits
  {
    constexpr auto l = target_type::encode<2>({1, 3}) + target_type::encode<2>({-2, 3});

    EXPECT_EQ(0b0110'0111, l.intv());
    EXPECT_EQ(0b1000'0000, l.carry());

    constexpr auto r = target_type::encode<2>({-4, -4});
    constexpr auto out = l - r;
    constexpr auto expected_int = 0b0'010'0'011;
    constexpr auto expected_carry = 0b1000'0000;

    EXPECT_EQ(expected_int, out.intv());
    EXPECT_EQ(expected_carry, out.carry());
  }

  // carries in LHS & RHS, no min error bits
  {
    constexpr auto v = target_type::encode<2>({-1, 3}) + target_type::encode<2>({-1, 3});

    EXPECT_EQ(0b0110'0110, v.intv());
    EXPECT_EQ(0b1000'0000, v.carry());

    constexpr auto l = v;
    constexpr auto r = v;
    constexpr auto out = l - r;
    constexpr auto expected_int = 0b0'000'0'000;
    constexpr auto expected_carry = 0b1000'0000;

    EXPECT_EQ(expected_int, out.intv());
    EXPECT_EQ(expected_carry, out.carry());
  }

  // carries in LHS & RHS, min error bits
  {
    constexpr auto v = target_type::encode<2>({-2, 3}) + target_type::encode<2>({-2, 3});

    EXPECT_EQ(0b0110'0100, v.intv());
    EXPECT_EQ(0b1000'0000, v.carry());

    constexpr auto l = v;
    constexpr auto r = v;
    constexpr auto out = l - r;
    constexpr auto expected_int = 0b0'000'0'000;
    constexpr auto expected_carry = 0b1000'0000;

    EXPECT_EQ(expected_int, out.intv());
    EXPECT_EQ(expected_carry, out.carry());
  }
}

TEST(Addition, NonOverflowingOneIntSevenBit)
{
  {
    constexpr auto l = multipleint::multiple_int<7, std::uint8_t>::encode<1>({0});
    constexpr auto r = multipleint::multiple_int<7, std::uint8_t>::encode<1>({0});
    constexpr auto s = l + r;

    EXPECT_EQ(0, s.intv());
    EXPECT_EQ(0, s.carry());
  }

  {
    constexpr auto l = multipleint::multiple_int<7, std::uint8_t>::encode<1>({0});
    constexpr auto r = multipleint::multiple_int<7, std::uint8_t>::encode<1>({1});
    constexpr auto s = l + r;

    EXPECT_EQ(1, s.intv());
    EXPECT_EQ(0, s.carry());
  }

  {
    constexpr auto l = multipleint::multiple_int<7, std::uint8_t>::encode<1>({1});
    constexpr auto r = multipleint::multiple_int<7, std::uint8_t>::encode<1>({0});
    constexpr auto s = l + r;

    EXPECT_EQ(1, s.intv());
    EXPECT_EQ(0, s.carry());
  }
}

TEST(Addition, OverflowingOneIntSevenBit)
{
  // Max container for 32 bits, carry bits are off
  constexpr auto l = std::numeric_limits<multipleint::multiple_int<7, std::uint8_t>>::max();
  EXPECT_EQ(0x3F, l.intv());

  constexpr auto r = multipleint::multiple_int<7, std::uint8_t>::encode<1>({1});
  EXPECT_EQ(0x01, r.intv());

  constexpr auto s = l + r;
  constexpr auto expectedi = 0x40;
  constexpr auto expectedc = 0x80;

  EXPECT_EQ(+expectedi, +s.intv()) << "Expected " << std::hex << +expectedi << ", got " << +s.intv() << "\n";
  EXPECT_EQ(+expectedc, +s.carry()) << "Expected " << std::hex << +expectedc << ", got " << +s.carry() << "\n";
}

TEST(Addition, PartiallyOverflowing4Ints7Bit)
{
  // Max container for 32 bits, carry bits are off

  constexpr auto l = std::numeric_limits<multipleint::multiple_int<7, std::uint32_t>>::max();
  constexpr auto expected_l = 0x3F'3F'3F'3F;

  EXPECT_EQ(expected_l, l.intv()) << "Expected " << std::hex << expected_l << ", got " << l.intv() << "\n";

  EXPECT_EQ(0x0, l.carry());

  // Will overflow on the lower two ints, but keep the upper two
  constexpr auto r = multipleint::multiple_int<7, std::uint32_t>::encode<4>({0x01, 0x01, 0x00, 0x00});
  constexpr auto s = l + r;

  constexpr auto expected_s = 0x3F'3F'40'40;
  EXPECT_EQ(expected_s, s.intv()) << "Expected " << std::hex << expected_s << ", got " << s.intv() << "\n";

  EXPECT_EQ(0x00'00'80'80, s.carry());
}

TEST(Max, TwoTwoBitInt8Bit)
{
  using target_type = multipleint::multiple_int<2, std::uint8_t>;

  constexpr auto l = target_type::encode<2>({0b00, 0b00});
  constexpr auto r = target_type::encode<2>({0b11, 0b11});

  constexpr auto m1 = max(l, r);
  constexpr auto m2 = max(r, l);

  constexpr auto expected = target_type::encode<2>({0b00, 0b00});

  EXPECT_EQ(+expected.intv(), +m1.intv())
      << "Expected " << std::hex << +expected.intv() << ", got " << +m1.intv() << "\n";
  EXPECT_EQ(+expected.intv(), +m2.intv())
      << "Expected " << std::hex << +expected.intv() << ", got " << +m2.intv() << "\n";
}

TEST(Max, TwoInt8Bit_1)
{
  using target_type = multipleint::multiple_int<3, std::uint8_t>;

  constexpr auto l = target_type::encode<2>({0b000, 0b001});
  constexpr auto r = target_type::encode<2>({0b000, 0b010});

  constexpr auto m1 = max(l, r);
  constexpr auto m2 = max(r, l);

  constexpr auto expected = target_type::encode<2>({0b000, 0b010});

  EXPECT_EQ(expected.intv(), m1.intv()) << "Expected " << std::hex << +expected.intv() << ", got " << +m1.intv()
                                        << "\n";
  EXPECT_EQ(expected.intv(), m2.intv()) << "Expected " << std::hex << +expected.intv() << ", got " << +m2.intv()
                                        << "\n";
}

TEST(Max, TwoInt8Bit_2)
{
  using target_type = multipleint::multiple_int<3, std::uint8_t>;

  constexpr auto l = target_type::encode<2>({0b000, 0b000});
  constexpr auto r = target_type::encode<2>({0b000, 0b001});

  constexpr auto m1 = max(l, r);
  constexpr auto m2 = max(r, l);

  constexpr auto expected = target_type::encode<2>({0b000, 0b001});

  EXPECT_EQ(expected.intv(), m1.intv()) << "Expected " << std::hex << +expected.intv() << ", got " << +m1.intv()
                                        << "\n";
  EXPECT_EQ(expected.intv(), m2.intv()) << "Expected " << std::hex << +expected.intv() << ", got " << +m2.intv()
                                        << "\n";
}

TEST(Max, FourInt16Bit)
{
  using target_type = multipleint::multiple_int<3, std::uint16_t>;

  constexpr auto l = target_type::encode<4>({0b001, 0b111, 0b110, 0b100});
  constexpr auto r = target_type::encode<4>({0b001, 0b111, 0b100, 0b101});

  constexpr auto m1 = max(l, r);
  constexpr auto m2 = max(r, l);

  constexpr auto expected = target_type::encode<4>({0b001, 0b111, 0b110, 0b101});
  EXPECT_EQ(expected.intv(), m1.intv());
  EXPECT_EQ(expected.intv(), m2.intv());
}

TEST(Max, FiveTwoBitInt16Bit)
{
  using target_type = multipleint::multiple_int<2, std::uint16_t>;

  constexpr auto l = target_type::encode<5>({0b00, 0b00, 0b00, 0b00, 0b00});
  constexpr auto r = target_type::encode<5>({0b11, 0b11, 0b11, 0b11, 0b11});

  constexpr auto m1 = max(l, r);
  constexpr auto m2 = max(r, l);

  constexpr auto expected = target_type::encode<5>({0b00, 0b00, 0b00, 0b00, 0b00});

  EXPECT_EQ(expected.intv(), m1.intv()) << "Expected " << std::hex << expected.intv() << ", got " << m1.intv() << "\n";
  EXPECT_EQ(expected.carry(), m1.carry())
      << "Expected " << std::hex << expected.carry() << ", got " << m1.carry() << "\n";
  EXPECT_EQ(expected.intv(), m2.intv()) << "Expected " << std::hex << expected.intv() << ", got " << m2.intv() << "\n";
  EXPECT_EQ(expected.carry(), m2.carry())
      << "Expected " << std::hex << expected.carry() << ", got " << m2.carry() << "\n";
}

TEST(Max, FourInt32Bit)
{
  using target_type = multipleint::multiple_int<7, std::uint32_t>;

  constexpr auto l = target_type::encode<4>({0x0E, 0x0E, 0x70, 0x70});
  constexpr auto r = target_type::encode<4>({0x7A, 0x0B, 0x7F, 0x7F});

  constexpr auto s = std::numeric_limits<target_type>::max() + target_type::encode<4>({0x00, 0x00, 0x01, 0x01});
  constexpr auto expected_s = target_type::encode<4>({0x3F, 0x3F, 0x40, 0x40});
  EXPECT_EQ(expected_s.intv(), s.intv()) << "Expected " << std::hex << expected_s.intv() << ", got " << s.intv();
  EXPECT_EQ(0x80'80'00'00, s.carry());

  constexpr auto max_lr1 = max(l, r);
  constexpr auto max_lr2 = max(r, l);
  constexpr auto exp_lr = target_type::encode<4>({0x0E, 0x0E, 0x7F, 0x7F});

  EXPECT_EQ(exp_lr.intv(), max_lr1.intv());
  EXPECT_EQ(0, max_lr1.carry());

  EXPECT_EQ(exp_lr.intv(), max_lr2.intv());
  EXPECT_EQ(0, max_lr2.carry());

  constexpr auto max_sr1 = max(s, r);
  constexpr auto max_sr2 = max(r, s);
  constexpr auto exp_sr = target_type::encode<4>({0x3F, 0x3F, 0x7F, 0x7F});

  EXPECT_EQ(exp_sr.intv(), max_sr1.intv());
  EXPECT_EQ(0x00'00'00'00, max_sr1.carry());

  EXPECT_EQ(exp_sr.intv(), max_sr2.intv());
  EXPECT_EQ(0x00'00'00'00, max_sr2.carry());

  constexpr auto max_sl1 = max(s, l);
  constexpr auto max_sl2 = max(l, s);
  constexpr auto exp_sl = target_type::encode<4>({0x3F, 0x3F, 0x70, 0x70});

  EXPECT_EQ(exp_sl.intv(), max_sl1.intv());
  EXPECT_EQ(0x00'00'00'00, max_sl1.carry());

  EXPECT_EQ(exp_sl.intv(), max_sl2.intv());
  EXPECT_EQ(0x00'00'00'00, max_sl2.carry());
}

TEST(Sum, FourInt8Bit)
{
  using target_type = multipleint::multiple_int<1, std::uint8_t>;

  constexpr auto in = target_type::encode<4>({1, 1, 1, 1});
  constexpr auto expected = -4;
  constexpr auto out = in.sum();

  EXPECT_EQ(+expected, +out);
}

TEST(Sum, EightInt16Bit)
{
  using target_type = multipleint::multiple_int<1, std::uint16_t>;

  constexpr auto in = target_type::encode<8>({1, 1, 1, 1, 1, 1, 1, 1});
  constexpr auto expected = -8;
  constexpr auto out = in.sum();

  EXPECT_EQ(+expected, +out);
}

TEST(Sum, EightInt32BitNonFull)
{
  using target_type = multipleint::multiple_int<3, std::uint32_t>;

  constexpr auto in = target_type::encode<8>({1, 2, 1, 2, 1, 2, 1, 2});
  constexpr auto expected = 12;
  constexpr auto out = in.sum();

  EXPECT_EQ(+expected, +out);
}

TEST(Max, MinInMax)
{
  using target_type = multipleint::multiple_int<3, std::uint8_t>;

  constexpr auto l = std::numeric_limits<target_type>::min();
  constexpr auto r = target_type::encode<2>({2, 1});

  constexpr auto expected = r;

  constexpr auto m1 = max(l, r);
  EXPECT_EQ(m1.intv(), expected.intv());
  EXPECT_EQ(m1.carry(), expected.carry());

  constexpr auto m2 = max(r, l);
  EXPECT_EQ(m2.intv(), expected.intv());
  EXPECT_EQ(m2.carry(), expected.carry());
}

TEST(Max, TwoPos)
{
  using target_type = multipleint::multiple_int<3, std::uint8_t>;

  constexpr auto l = target_type::encode<2>({1, 2});
  constexpr auto r = target_type::encode<2>({2, 1});

  constexpr auto expected = target_type::encode<2>({2, 2});

  constexpr auto m1 = max(l, r);
  EXPECT_EQ(m1.intv(), expected.intv());
  EXPECT_EQ(m1.carry(), expected.carry());

  constexpr auto m2 = max(r, l);
  EXPECT_EQ(m2.intv(), expected.intv());
  EXPECT_EQ(m2.carry(), expected.carry());
}

TEST(Max, TwoNeg)
{
  using target_type = multipleint::multiple_int<3, std::uint8_t>;

  constexpr auto l = target_type::encode<2>({-1, -2});
  constexpr auto r = target_type::encode<2>({-2, -1});

  constexpr auto expected = target_type::encode<2>({-1, -1});

  constexpr auto m1 = max(l, r);
  EXPECT_EQ(m1.intv(), expected.intv());
  EXPECT_EQ(m1.carry(), expected.carry());

  constexpr auto m2 = max(r, l);
  EXPECT_EQ(m2.intv(), expected.intv());
  EXPECT_EQ(m2.carry(), expected.carry());
}

TEST(Max, DiffSigns)
{
  using target_type = multipleint::multiple_int<3, std::uint8_t>;

  constexpr auto l = target_type::encode<2>({-1, 2});
  constexpr auto r = target_type::encode<2>({2, -1});

  constexpr auto expected = target_type::encode<2>({2, 2});

  constexpr auto m1 = max(l, r);
  EXPECT_EQ(m1.intv(), expected.intv());
  EXPECT_EQ(m1.carry(), expected.carry());

  constexpr auto m2 = max(r, l);
  EXPECT_EQ(m2.intv(), expected.intv());
  EXPECT_EQ(m2.carry(), expected.carry());
}

TEST(Maxima, OnlyMins)
{
  {
    using target_type = multipleint::multiple_int<3, std::uint8_t>;

    constexpr auto l = target_type::encode<1>({-4});
    constexpr auto r = target_type::encode<1>({-4});

    constexpr auto expected = target_type::encode<1>({-4});

    constexpr auto lrresult = max(l, r);
    EXPECT_EQ(expected.intv(), lrresult.intv());
    EXPECT_EQ(expected.carry(), lrresult.carry());

    constexpr auto rlresult = max(r, l);
    EXPECT_EQ(expected.intv(), rlresult.intv());
    EXPECT_EQ(expected.carry(), rlresult.carry());
  }
}

TEST(Maxima, BothPos)
{
  using target_type = multipleint::multiple_int<3, std::uint8_t>;

  constexpr auto l = target_type::encode<1>({1});
  constexpr auto r = target_type::encode<1>({2});

  constexpr auto expected = target_type::encode<1>({2});

  constexpr auto lrresult = max(l, r);
  EXPECT_EQ(expected.intv(), lrresult.intv());

  constexpr auto rlresult = max(r, l);
  EXPECT_EQ(expected.intv(), rlresult.intv());
}

TEST(Maxima, BothNeg)
{
  using target_type = multipleint::multiple_int<3, std::uint8_t>;

  constexpr auto l = target_type::encode<1>({-1});
  constexpr auto r = target_type::encode<1>({-2});

  constexpr auto expected = target_type::encode<1>({-1});

  constexpr auto lrresult = max(l, r);
  EXPECT_EQ(expected.intv(), lrresult.intv());

  constexpr auto rlresult = max(r, l);
  EXPECT_EQ(expected.intv(), rlresult.intv());
}

TEST(Maxima, DifferentSigns)
{
  using target_type = multipleint::multiple_int<3, std::uint8_t>;

  constexpr auto l = target_type::encode<1>({-1});
  constexpr auto r = target_type::encode<1>({2});

  constexpr auto expected = target_type::encode<1>({2});

  constexpr auto lrresult = max(l, r);
  EXPECT_EQ(expected.intv(), lrresult.intv());

  constexpr auto rlresult = max(r, l);
  EXPECT_EQ(expected.intv(), rlresult.intv());
}

TEST(Maxima, Mins)
{
  using target_type = multipleint::multiple_int<3, std::uint8_t>;

  {
    constexpr auto l = target_type::encode<1>({-4});
    constexpr auto r = target_type::encode<1>({-1});

    constexpr auto expected = target_type::encode<1>({-1});

    constexpr auto lrresult = max(l, r);
    EXPECT_EQ(expected.intv(), lrresult.intv());

    constexpr auto rlresult = max(r, l);
    EXPECT_EQ(expected.intv(), rlresult.intv());
  }

  {
    constexpr auto l = target_type::encode<1>({-4});
    constexpr auto r = target_type::encode<1>({+1});

    constexpr auto expected = target_type::encode<1>({+1});

    constexpr auto lrresult = max(l, r);
    EXPECT_EQ(expected.intv(), lrresult.intv());

    constexpr auto rlresult = max(r, l);
    EXPECT_EQ(expected.intv(), rlresult.intv());
  }
}

TEST(SingleMax, TwoInt8Bit)
{
  using target_type = multipleint::multiple_int<3, std::uint8_t>;

  {
    constexpr auto t = target_type::encode<2>({0b111, 0b000});
    constexpr auto r = t.max();

    EXPECT_EQ(0b0000, r);
  }

  {
    constexpr auto t = target_type::encode<2>({0b001, 0b100});
    constexpr auto r = t.max();

    EXPECT_EQ(0b0001, r);
  }

  {
    constexpr auto t = target_type::encode<2>({0b011, 0b100});
    constexpr auto r = t.max();

    EXPECT_EQ(0b0011, r);
  }

  {
    constexpr auto t = target_type::encode<2>({0b011, 0b111});
    constexpr auto r = t.max();

    EXPECT_EQ(0b0011, r);
  }

  {
    constexpr auto t = target_type::encode<2>({0b000, 0b100});
    constexpr auto r = t.max();

    EXPECT_EQ(0b0000, r);
  }
}

TEST(SingleMax, FiveInt16Bit)
{
  using target_type = multipleint::multiple_int<2, std::uint16_t>;

  constexpr auto in = target_type::encode<5>({0b01, 0b10, 0b11, 0b01, 0b00});
  constexpr auto out = in.max();

  EXPECT_EQ(0b001, out);
}

TEST(SingleMax, FiveInt32Bit)
{
  using target_type = multipleint::multiple_int<5, std::uint32_t>;

  constexpr auto in = target_type::encode<5>({0b00000, 0b10000, 0b11111, 0b01101, 0b01110});
  constexpr auto out = in.max();

  EXPECT_EQ(0b01110, out);
}