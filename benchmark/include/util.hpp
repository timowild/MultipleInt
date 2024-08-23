#pragma once

#include <execution>

inline constexpr auto exec_policy = std::execution::par_unseq;

template<typename T>
using Container = std::vector<T>;

// https://stackoverflow.com/a/63821008
namespace detail
{

template<size_t, typename T>
constexpr T&& identity(T&& x)
{
  return std::forward<T>(x);
}

template<typename T, size_t... Indices>
constexpr auto array_repeat_impl(T&& x, std::index_sequence<Indices...>)
{
  return std::array {identity<Indices>(x)...};
}

}  // namespace detail

template<size_t N, typename T>
constexpr auto array_repeat(T&& x) -> std::array<T, N>
{
  return detail::array_repeat_impl(std::forward<T>(x), std::make_index_sequence<N>());
}
