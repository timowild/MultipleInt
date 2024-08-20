#pragma once

#include <cinttypes>
#include <type_traits>

namespace multipleint::detail
{

// Set every BitWidth'th bit
template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
struct _carry_mask
{
  static constexpr BackingStorage pattern = static_cast<BackingStorage>(1) << BitWidth;

  static constexpr BackingStorage value =
      pattern << ((IntCount - 1) * (BitWidth + 1)) | _carry_mask<IntCount - 1, BitWidth, BackingStorage>::value;
};

template<std::size_t BitWidth, typename BackingStorage>
struct _carry_mask<0, BitWidth, BackingStorage>
{
  static constexpr BackingStorage value = 0;
};

template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
constexpr BackingStorage _carry_mask_v = _carry_mask<IntCount, BitWidth, BackingStorage>::value;

// Set n bits, n + 1'th bit is for the carry, and above the IntCount * BitWidth
// bit is only 0s
template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
struct _int_mask
{
  static constexpr BackingStorage pattern = (static_cast<BackingStorage>(1) << BitWidth) - 1;

  static constexpr BackingStorage value =
      (pattern << ((IntCount - 1) * (BitWidth + 1))) | _int_mask<IntCount - 1, BitWidth, BackingStorage>::value;
};

template<std::size_t BitWidth, typename BackingStorage>
struct _int_mask<0, BitWidth, BackingStorage>
{
  static constexpr BackingStorage value = 0;
};

template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
constexpr BackingStorage _int_mask_v = _int_mask<IntCount, BitWidth, BackingStorage>::value;

// Set upper sizeof(BackingStorage) - (IntCount * BitWidth) bits,
// remainder is only 0s
template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
struct _empty_mask
{
  static constexpr BackingStorage value = IntCount * (BitWidth + 1) == sizeof(BackingStorage) * 8
      // ints and their carry bits take up entire storage space, empty
      // mask is itself, "empty"
      ? static_cast<BackingStorage>(0)
      // bits need to be padded
      : ~((static_cast<BackingStorage>(1) << (IntCount * (BitWidth + 1))) - 1);
};

template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
constexpr BackingStorage _empty_mask_v = _empty_mask<IntCount, BitWidth, BackingStorage>::value;

template<std::size_t IntCount, std::size_t OldBitWidth, std::size_t NewBitWidth, typename BackingStorage>
struct truncation_mask
{
  static constexpr BackingStorage pattern = (static_cast<BackingStorage>(1) << (NewBitWidth + 1)) - 1;

  static constexpr BackingStorage value = (pattern << ((IntCount - 1) * (OldBitWidth + 1)))
      | truncation_mask<IntCount - 1, OldBitWidth, NewBitWidth, BackingStorage>::value;
};

template<std::size_t OldBitWidth, std::size_t NewBitWidth, typename BackingStorage>
struct truncation_mask<0, OldBitWidth, NewBitWidth, BackingStorage>
{
  static constexpr BackingStorage value = 0;
};

template<std::size_t IntCount, std::size_t OldBitWidth, std::size_t NewBitWidth, typename BackingStorage>
static constexpr auto truncation_mask_v = truncation_mask<IntCount, OldBitWidth, NewBitWidth, BackingStorage>::value;

template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
struct sign_mask
{
  static constexpr BackingStorage pattern = static_cast<BackingStorage>(1) << (BitWidth - 1);

  static constexpr BackingStorage value =
      (pattern << ((IntCount - 1) * (BitWidth + 1))) | sign_mask<IntCount - 1, BitWidth, BackingStorage>::value;
};

template<std::size_t BitWidth, typename BackingStorage>
struct sign_mask<0, BitWidth, BackingStorage>
{
  static constexpr BackingStorage value = 0;
};

template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
static constexpr auto _sign_mask_v = sign_mask<IntCount, BitWidth, BackingStorage>::value;

// Next widest uint_t-datatype
template<class>
struct _next_widest;

template<>
struct _next_widest<uint8_t>
{
  using type = uint16_t;
};

template<>
struct _next_widest<uint16_t>
{
  using type = uint32_t;
};

template<>
struct _next_widest<uint32_t>
{
  using type = uint64_t;
};

template<>
struct _next_widest<uint64_t>
{
  // There is no bigger uint_t-datatype
  using type = uint64_t;
};

// Mask all even (or odd) integer with 1s
template<int Index, std::size_t BitWidth, typename BackingStorage>
struct _int_pattern
{
  using IntMask = typename detail::_int_mask<Index, BitWidth, BackingStorage>;

  static constexpr BackingStorage value =
      IntMask::pattern << (Index * (BitWidth + 1)) | _int_pattern<Index - 2, BitWidth, BackingStorage>::value;
};

template<std::size_t BitWidth, typename BackingStorage>
struct _int_pattern<1, BitWidth, BackingStorage>
{
  using IntMask = typename detail::_int_mask<1, BitWidth, BackingStorage>;

  static constexpr BackingStorage value = IntMask::pattern << (BitWidth + 1);
};

template<std::size_t BitWidth, typename BackingStorage>
struct _int_pattern<0, BitWidth, BackingStorage>
{
  static constexpr BackingStorage value = _int_pattern<1, BitWidth, BackingStorage>::IntMask::pattern;
};

// Mask all bits other than the sign bits with 1s
template<int IntCount, std::size_t SmallerBitWidth, typename SmallerBackingStorage, typename BackingStorage>
struct _sign_pattern
{
  using smaller_int_mask = detail::_int_mask<1, SmallerBitWidth, SmallerBackingStorage>;
  using bigger_int_mask = detail::_int_mask<1, 2 * SmallerBitWidth + 1, BackingStorage>;

  static constexpr BackingStorage pattern = bigger_int_mask::pattern ^ smaller_int_mask::pattern;

  static constexpr BackingStorage value = pattern << ((IntCount - 1) * (2 * SmallerBitWidth + 2))
      | _sign_pattern<IntCount - 1, SmallerBitWidth, SmallerBackingStorage, BackingStorage>::value;
};

template<std::size_t SmallerBitWidth, typename SmallerBackingStorage, typename BackingStorage>
struct _sign_pattern<0, SmallerBitWidth, SmallerBackingStorage, BackingStorage>
{
  static constexpr BackingStorage value = 0;
};

template<std::size_t IntCount, std::size_t BitWidth, typename BackingStorage>
struct _multiple_int_traits
{
  static_assert(BitWidth > 0, "No such thing as integers with 0 bits (at least, not here!)");

  static_assert(IntCount >= 1, "At least one integer must be stored");

  // Masks all the bits reserved for the actual values
  static constexpr BackingStorage int_mask = _int_mask_v<IntCount, BitWidth, BackingStorage>;

  // Mask only the singular padding carry bits
  static constexpr BackingStorage carry_mask = _carry_mask_v<IntCount, BitWidth, BackingStorage>;

  static constexpr BackingStorage empty_mask = _empty_mask_v<IntCount, BitWidth, BackingStorage>;

  static constexpr BackingStorage sign_mask = _sign_mask_v<IntCount, BitWidth, BackingStorage>;

  template<typename T>
  using next_widest = typename _next_widest<T>::type;

  // Masks upper N - BitWidth bits of value
};

template<class IndexSequence, std::size_t Add>
struct increase_index_sequence;

template<std::size_t... I, std::size_t Add>
struct increase_index_sequence<std::index_sequence<I...>, Add>
{
  using type = std::index_sequence<(I + Add)...>;
};

/* clang-format off */
/// index sequence of type size_t of the range [Start, End)
template<std::size_t Start, std::size_t End>
requires(Start <= End)
using index_sequence_from_to = typename increase_index_sequence<std::make_index_sequence<End - Start>, Start>::type;
/* clang-format on */

}  // namespace multipleint::detail