#pragma once

#include <algorithm>
#include <execution>
#include <iterator>
#include <numeric>

namespace bench
{

template<class Exec, class InputIterator1, class InputIterator2, class OutputIterator>
constexpr void xpy(Exec&& exec, InputIterator1 x_b, InputIterator1 x_e, InputIterator2 y_b, OutputIterator z_b)
{
  using X = typename std::iterator_traits<InputIterator1>::value_type;
  using Y = typename std::iterator_traits<InputIterator2>::value_type;
  using Z = typename std::iterator_traits<OutputIterator>::value_type;

  std::transform(std::forward<Exec>(exec), x_b, x_e, y_b, z_b, [](const X& x, const Y& y) -> Z { return x + y; });
}

template<class Exec, class InputIterator1, class InputIterator2, class OutputIterator>
constexpr void elemwise_max(Exec&& exec, InputIterator1 x_b, InputIterator1 x_e, InputIterator2 y_b, OutputIterator z_b)
{
  using X = typename std::iterator_traits<InputIterator1>::value_type;
  using Y = typename std::iterator_traits<InputIterator2>::value_type;
  using Z = typename std::iterator_traits<OutputIterator>::value_type;
  using std::max;

  std::transform(std::forward<Exec>(exec), x_b, x_e, y_b, z_b, [](const X& x, const Y& y) -> Z { return max(x, y); });
}

template<class Exec, class InputIterator, typename T>
constexpr auto sum_red(Exec&& exec, InputIterator vals_b, InputIterator vals_e, T init)
{
  return std::reduce(std::forward<Exec>(exec), vals_b, vals_e, init);
}

// Alternative implementation with purely integer based reduction
template<class Exec, class InputIterator, typename T>
constexpr auto sum_red_alt(Exec&& exec, InputIterator vals_b, InputIterator vals_e, T init)
{
  return std::transform_reduce(
      std::forward<Exec>(exec), vals_b, vals_e, init, std::plus<T>(), [](const auto& x) -> T { return x.sum(); });
}

template<class Exec, class InputIterator>
constexpr auto max_red(Exec&& exec, InputIterator vals_b, InputIterator vals_e)
{
  using T = typename std::iterator_traits<InputIterator>::value_type;
  using std::max;

  return std::reduce(std::forward<Exec>(exec),
                     vals_b,
                     vals_e,
                     std::numeric_limits<T>::lowest(),
                     [](const T& x, const T& y) -> T { return max(x, y); });
}

// Alternative implementation with purely integer based reduction
template<class Exec, class InputIterator, typename T>
constexpr auto max_red_alt(Exec&& exec, InputIterator vals_b, InputIterator vals_e, T init)
{
  using std::max;

  return std::transform_reduce(
      std::forward<Exec>(exec),
      vals_b,
      vals_e,
      init,
      [](const T& x, const T& y) -> T { return max(x, y); },
      [](const auto& x) -> T { return x.max(); });
}

}  // namespace bench