#pragma once

#include <array>
#include <type_traits>

#include "mitraits.hpp"

#ifndef CHAR_BIT
#  define CHAR_BIT 8
#endif

namespace multipleint
{

template<std::size_t BitWidth, std::unsigned_integral BackingStorage>
class multiple_int
{
public:
  // Every Int has an additional carry/overflow bit
  static constexpr int IntCount = (8 * sizeof(BackingStorage)) / (BitWidth + 1);

  using traits = detail::_multiple_int_traits<IntCount, BitWidth, BackingStorage>;

  friend std::numeric_limits<multiple_int<BitWidth, BackingStorage>>;  // needed for private ctor

  // We cannot access the private member in the conversion constructor because we
  // are here in a templated class.
  friend multiple_int<2 * BitWidth + 1, typename traits::template next_widest<BackingStorage>>;

private:
  constexpr explicit multiple_int(BackingStorage value)
      : value_ {value}
  {
  }

public:
  // Default ctor = all zeros
  constexpr multiple_int() = default;

  /* clang-format off */
  template<std::size_t SmallerBitWidth, typename SmallerBackingStorage>
  requires(2 * SmallerBitWidth + 1 == BitWidth
           && 2 * sizeof(SmallerBackingStorage) == sizeof(BackingStorage))
  constexpr multiple_int(multiple_int<SmallerBitWidth, SmallerBackingStorage> halfSizeMultipleInt)
      : value_ {0}
  /* clang-format on */
  {
    using lower_multiple_int = multiple_int<SmallerBitWidth, SmallerBackingStorage>;

    constexpr auto even_int_mask = detail::_int_pattern < (IntCount % 2 == 0) ? IntCount - 2 : IntCount - 1,
                   SmallerBitWidth, SmallerBackingStorage > ::value;
    constexpr auto odd_int_mask = detail::_int_pattern < (IntCount % 2 == 0) ? IntCount - 1 : IntCount - 2,
                   SmallerBitWidth, SmallerBackingStorage > ::value;

    // Mask all bits other than the sign bits with 1s
    constexpr auto signMaskShiftedByOne =
        detail::_sign_pattern<IntCount, SmallerBitWidth, SmallerBackingStorage, BackingStorage>::value;

    // E.g. −2 in 3b is 0b110 -> Mask: 0000; 2 in 3b is 0b010 -> Mask: 1000
    constexpr auto invertedAndShiftedSignBitMask =
        (lower_multiple_int::traits::carry_mask
         | static_cast<BackingStorage>(lower_multiple_int::traits::carry_mask) << (IntCount * (SmallerBitWidth + 1)))
        ^ traits::carry_mask;

    // Transfer all small ints with EVEN indices
    value_ |= halfSizeMultipleInt.value_ & even_int_mask;

    // Transfer all small ints with ODD indices
    value_ |= ((static_cast<BackingStorage>(halfSizeMultipleInt.value_ & odd_int_mask))
               << ((IntCount % 2 == 0) ? (IntCount - 1) : (IntCount)) * (SmallerBitWidth + 1));

    // Add sign mask to the result (Negative numbers become now negative again)
    value_ += signMaskShiftedByOne;

    // If the number was a positive number, we have to add the inverted and shifted sign
    // bit mask
    value_ += (((~value_) & traits::int_mask) << 1) & invertedAndShiftedSignBitMask;

    // Reset all carry (overflow)-bits
    value_ &= traits::int_mask;
  }

  constexpr multiple_int(const multiple_int<BitWidth, BackingStorage>&) = default;
  constexpr multiple_int& operator=(const multiple_int<BitWidth, BackingStorage>&) = default;
  constexpr multiple_int(multiple_int<BitWidth, BackingStorage>&&) = default;
  constexpr multiple_int& operator=(multiple_int<BitWidth, BackingStorage>&&) = default;

private:
  BackingStorage value_;

public:
  /* clang-format off */
  template<std::size_t ExtractNumberAtIndex, typename T = std::make_signed_t<BackingStorage>>
  requires(ExtractNumberAtIndex < IntCount)
  constexpr auto extract() const -> T
  /* clang-format on */
  {
    // Create a mask with #bit-width bits set to one
    constexpr auto mask = (static_cast<BackingStorage>(1) << BitWidth) - 1;

    // Extract one number from the internal storage
    const auto val = [this]() constexpr
    {
      if constexpr (ExtractNumberAtIndex == 0)
        return (value_ & mask);
      else
        return ((value_ >> (ExtractNumberAtIndex * (BitWidth + 1))) & mask);
    }();

    // Create a mask depending on the MSB of the extracted number
    const auto neg_mask = ((~((val >> (BitWidth - 1)) - 1)) << BitWidth);

    return (val | neg_mask);
  }

  /* clang-format off */
  template<std::size_t EncodeNumberAtIndex> 
  requires(EncodeNumberAtIndex < IntCount)
  constexpr auto encode(int input) -> void
  /* clang-format on */
  {
    constexpr auto mask = (static_cast<BackingStorage>(1) << BitWidth) - 1;
    constexpr auto shift = (BitWidth + 1) * EncodeNumberAtIndex;
    constexpr auto delete_mask = ~(mask << shift);

    // Clear value bits at EncodeNumberAtIndex
    value_ &= delete_mask;

    // Insert value
    if constexpr (shift == 0) {
      value_ |= (input & mask);
    } else {
      value_ |= ((input & mask) << shift);
    }
  }

  /* clang-format off */
  template<std::size_t AtMostIntCount> 
  requires(AtMostIntCount > 0 && AtMostIntCount <= IntCount)
  static constexpr auto encode(const std::array<int, AtMostIntCount>& input) -> multiple_int<BitWidth, BackingStorage>
  /* clang-format on */
  {
    // Create a mask with #bit-width bits set to one
    constexpr auto mask = (static_cast<BackingStorage>(1) << BitWidth) - 1;

    BackingStorage value_ {};

    if constexpr (AtMostIntCount > 1) {
      [&input, &value_]<std::size_t... Idx>(std::index_sequence<Idx...>) constexpr
      {
        const auto f = [&input, &value_]<std::size_t I>() constexpr
        {
          // Encoding numbers are inserted in reverse order
          // -> input[0] is stored in the "lower value" bits

          // Insert value
          value_ |= (std::get<AtMostIntCount - 1 - I>(input) & mask);
          // Shift by bit width + 1 (carry bit)
          value_ <<= (BitWidth + 1);
        };

        (f.template operator()<Idx>(), ...);
      }(std::make_index_sequence<AtMostIntCount - 1> {});
    }

    // Don't shift the last value (i.e. first value in array)
    value_ |= (std::get<0>(input) & mask);

    return multiple_int<BitWidth, BackingStorage> {value_};
  }

  /* clang-format off */
  template<std::size_t AtMostIntCount> 
  requires(AtMostIntCount > 0 && AtMostIntCount <= IntCount)
  constexpr auto decode() const -> std::array<int, AtMostIntCount>
  /* clang-format on */
  {
      /* clang-format off */
    return [this]<std::size_t... Idx>(std::index_sequence<Idx...>) constexpr 
      { 
      return std::array<int, AtMostIntCount> {this->extract<Idx, int>()...};
      }(std::make_index_sequence<AtMostIntCount> {});
      /* clang-format on */
  }

  constexpr auto sum() const -> std::make_signed_t<BackingStorage>
  {
    using value_type = std::make_signed_t<BackingStorage>;

    /* clang-format off */
    return [this]<std::size_t... Idx>(std::index_sequence<Idx...>) constexpr
    {
      return value_type ((this->extract<Idx, value_type>() + ...));
    }(std::make_index_sequence<IntCount> {});
    /* clang-format on */
  }

  constexpr friend auto max(multiple_int<BitWidth, BackingStorage> lhs, multiple_int<BitWidth, BackingStorage> rhs)
      -> multiple_int<BitWidth, BackingStorage>
  {
    using value_type = multiple_int<BitWidth, BackingStorage>;

    auto a = -value_type {rhs.intv()};
    auto diffA = value_type {lhs.intv()} + a;
    auto b = -value_type {lhs.intv()};
    auto diffB = value_type {rhs.intv()} + b;

    // Extract the carry bits and shift it to the sign position
    auto carries_at_signA = (diffA.carry() >> 1);
    auto carries_at_signB = (diffB.carry() >> 1);

    auto signs = (((diffA.value_ & traits::sign_mask) & ~carries_at_signA)
                  | ((diffB.value_ & traits::sign_mask) & carries_at_signB))
        >> (BitWidth - 1);

    // Generate blocks of 0s or 1s depeding on the sign bit
    auto max_mask = (signs + traits::int_mask) & traits::int_mask;
    max_mask |= (max_mask & traits::sign_mask) << 1;

    // Select the max value with max_mask and store it in result
    BackingStorage result = ((lhs.value_ & max_mask) | (rhs.value_ & ~max_mask));

    return value_type {result};
  }

  // Returns the maximum of all stored values
  constexpr auto max() const -> std::make_signed_t<BackingStorage>
  {
    using value_type = std::make_signed_t<BackingStorage>;

    // First value is current maximum
    value_type result {extract<0>()};

    if constexpr (IntCount >= 2) {
      /* clang-format off */
      [&result, this]<std::size_t... Idx>(std::index_sequence<Idx...>) constexpr
      {
        // result = std::max({result, this->extract<Idx, value_type>()...});

        const auto f = [&result, this]<std::size_t I>()
        {
            // https://graphics.stanford.edu/~seander/bithacks.html 
            // Compute the maximum of two integers without branching
            const auto diff = result - this->extract<I, value_type>();

            result = result - (diff & (diff >> (sizeof(value_type) * CHAR_BIT - 1)));
        };

        (f.template operator()<Idx>(), ...);

      }(detail::index_sequence_from_to<1, IntCount> {});
      /* clang-format on */
    }

    return result;
  }

  /* clang-format off */
  template<std::size_t SmallerBitWidth, typename SmallerBackingStorage>
  requires(2 * SmallerBitWidth + 1 == BitWidth && 
           2 * sizeof(SmallerBackingStorage) == sizeof(BackingStorage))
  constexpr explicit operator multiple_int<SmallerBitWidth, SmallerBackingStorage>() const
  /* clang-format on */
  {
    using source_type = multiple_int<BitWidth, BackingStorage>;
    using target_type = multiple_int<SmallerBitWidth, SmallerBackingStorage>;

    constexpr auto trunc = detail::truncation_mask_v<IntCount, BitWidth, SmallerBitWidth, BackingStorage>;

    auto truncated = value_ & trunc;

    // rounds up odd counts of numbers, as these are the 0th, 2nd, 4th etc.
    constexpr auto lower_shift = (IntCount % 2 == 0) ? IntCount / 2 : (IntCount + 1) / 2;

    // matches lower "half" of truncated numbers
    constexpr auto lower = (static_cast<BackingStorage>(1) << (lower_shift * (BitWidth + 1))) - 1;

    // matches upper "half" of truncated numbers
    constexpr auto upper = ~lower & ~source_type::traits::empty_mask;

    // extracts integers in the lower "half"
    auto lower_half = static_cast<SmallerBackingStorage>(truncated & lower);

    // extracts integers in the upper "half"
    auto upper_half = truncated & upper;

    constexpr auto upper_shift_down = ((IntCount % 2 == 0) ? (IntCount - 1) : (IntCount)) * (BitWidth + 1) / 2;
    auto upper_half2 = static_cast<SmallerBackingStorage>(upper_half >> upper_shift_down);

    // merge results
    return target_type {static_cast<SmallerBackingStorage>(lower_half | upper_half2)};
  }

  constexpr BackingStorage intv() const { return this->value_ & traits::int_mask; }

  constexpr BackingStorage carry() const { return this->value_ & traits::carry_mask; }

  constexpr auto operator+(multiple_int<BitWidth, BackingStorage> rhs) const -> multiple_int<BitWidth, BackingStorage>
  {
    // Use intv instead of the raw value to avoid adding carry bits, which
    // would "bleed" their overflow into the LSB of the following integer
    auto lhsi = this->intv();
    auto rhsi = rhs.intv();
    auto sumi = static_cast<BackingStorage>(lhsi + rhsi);

    auto lhs_signs = lhsi & traits::sign_mask;
    auto rhs_signs = rhsi & traits::sign_mask;
    auto sum_signs = sumi & traits::sign_mask;

    auto error = (lhs_signs ^ sum_signs) & (rhs_signs ^ sum_signs);

    // zero out carry bits, replace with calculated carry bits
    sumi &= traits::int_mask;
    sumi |= (error << 1) | this->carry() | rhs.carry();

    return multiple_int<BitWidth, BackingStorage> {sumi};
  }

  constexpr auto operator-(multiple_int<BitWidth, BackingStorage> rhs) const -> multiple_int<BitWidth, BackingStorage>
  {
    // Carry bit that only occur when attempting to negate the min.
    auto inv_rhs = -rhs;
    auto inv_carries = (inv_rhs.carry() - rhs.carry()) & traits::carry_mask;

    auto orig_sum = (*this) + inv_rhs;

    // turn off bits that were set due to negation error
    orig_sum.value_ &= ~inv_carries;

    // turn on if they were set anyway in the original rhs and rhs
    orig_sum.value_ |= this->carry() | rhs.carry();

    return orig_sum;
  }

  constexpr auto operator-() const -> multiple_int<BitWidth, BackingStorage>
  {
    constexpr auto add_one_mask = static_cast<BackingStorage>((traits::carry_mask << 1) | 1) & ~traits::empty_mask;

    BackingStorage tint = this->intv();
    BackingStorage negated = ~tint;

    // apply and twice to avoid carry bits when handling 0
    BackingStorage un_minus_wcarry = ((negated & traits::int_mask) + add_one_mask);

    BackingStorage no_sign_change = (~(tint ^ un_minus_wcarry) & traits::sign_mask) << 1;
    BackingStorage errors = no_sign_change ^ (un_minus_wcarry & traits::carry_mask);

    BackingStorage un_minus = un_minus_wcarry & traits::int_mask;
    BackingStorage with_errors = un_minus | errors | this->carry();

    return multiple_int<BitWidth, BackingStorage> {with_errors};
  }
};
}  // namespace multipleint