#pragma once

#include "core.h"

#include <type_traits>

namespace compile_time_list
{

template <typename T, T A, T B, unsigned N>
struct fibonacci
{
  using item_type = T;

  template <T X, T Y, unsigned I>
  struct state
  {
    static constexpr T current = X;
    static constexpr T next = Y;
    static constexpr unsigned index = I;
  };

  using init = state<A, B, 0>;

  template <typename S>
  using can_proceed = std::bool_constant<S::index != N>;

  template <typename S>
  using get_item = std::integral_constant<T, S::current>;

  template <typename S>
  struct step
  {
    using result = state<
      S::next,
      S::next + S::current,
      S::index + 1>;
  };
};

}

