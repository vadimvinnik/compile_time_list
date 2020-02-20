#pragma once

#include "core.h"

#include "iota.h"

#include <type_traits>

namespace compile_time_list
{

template <typename T, unsigned N>
struct primes
{
  using item_type = T;

  using init = prepend_t<generate_t<iota<T, 3, N, 2>>, 2>;

  template <typename S>
  using can_proceed = std::bool_constant<!is_empty_v<S>>;

  template <typename S>
  using get_item = std::integral_constant<T, head_v<S>>;

  template <typename S>
  struct step
  {
  private:
    template <unsigned X>
    struct predicate : std::bool_constant<(X % head_v<S> == 0)> {};

  public:
    using result = typename partition<S, predicate>::right;
  };
};

}

