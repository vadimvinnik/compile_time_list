#pragma once

#include "core.h"

#include <type_traits>

namespace compile_time_list
{

template <typename T, T A, T B, T D>
struct iota
{
  using item_type = T;

  using init = std::integral_constant<T, A>;

  template <typename X>
  using can_proceed = std::bool_constant<(X::value < B)>;

  template <typename X>
  using get_item = X;

  template <typename X>
  struct step
  {
    using result = std::integral_constant<T, X::value + D>;
  };
};

}

