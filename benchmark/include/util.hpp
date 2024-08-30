#pragma once

#include <execution>

inline constexpr auto exec_policy = std::execution::par_unseq;

template<typename T>
using Container = std::vector<T>;
