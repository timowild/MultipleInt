#pragma once

#include <limits>
#include <type_traits>

#include "mi.hpp"
#include "mitraits.hpp"

template<std::size_t BitWidth, typename BackingStorage>
struct std::numeric_limits<multipleint::multiple_int<BitWidth, BackingStorage>>
{
private:
  using value_type = multipleint::multiple_int<BitWidth, BackingStorage>;

  using traits = typename value_type::traits;

public:
  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr bool is_integer = true;
  static constexpr bool is_exact = true;

  static constexpr bool has_infinity = false;
  static constexpr bool has_quiet_NaN = false;
  static constexpr bool has_signaling_NaN = false;

  static constexpr bool has_denorm = false;
  static constexpr bool has_denorm_loss = false;

  static constexpr std::float_round_style round_style = std::round_toward_zero;

  static consteval auto min() noexcept -> value_type
  {
    constexpr auto min = traits::sign_mask;
    return value_type {min};
  }

  static consteval auto lowest() noexcept -> value_type { return min(); }

  static consteval auto max() noexcept -> value_type
  {
    constexpr auto max = traits::int_mask & ~traits::sign_mask;
    return value_type {max};
  }
};