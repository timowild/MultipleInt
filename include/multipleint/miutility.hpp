#pragma once

#include <type_traits>

namespace multipleint::detail
{
template<class IndexSequence, std::size_t Add>
struct _increase_index_sequence;

template<std::size_t... I, std::size_t Add>
struct _increase_index_sequence<std::index_sequence<I...>, Add>
{
  using type = std::index_sequence<(I + Add)...>;
};

/* clang-format off */
/// index sequence of type size_t of the range [Start, End)
template<std::size_t Start, std::size_t End>
requires(Start <= End)
using index_sequence_from_to = typename _increase_index_sequence<std::make_index_sequence<End - Start>, Start>::type;
/* clang-format on */

template<std::size_t N, typename T>
constexpr auto array_repeat(T x) -> std::array<T, N>
{
  return [=]<std::size_t... Idx>(std::index_sequence<Idx...>)
  {
    const auto identity = [=]<std::size_t>() constexpr { return x; };

    return std::array<T, N> {identity.template operator()<Idx>()...};
  }(std::make_index_sequence<N>());
}
}  // namespace multipleint::detail