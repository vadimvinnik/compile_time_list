#pragma once

namespace compile_time_list
{

template <typename T, T... Xs>
struct list
{
  using item_type = T;

  static constexpr auto size = sizeof...(Xs);
};

template <typename U>
using item_t = typename U::item_type;

template <typename U>
constexpr auto size()
{
  return U::size;
}

template <typename U, item_t<U> X>
struct append; // never reach this case

template <typename T, T... Xs, T X>
struct append<list<T, Xs...>, X>
{
  using type = list<T, Xs..., X>;
};

template <typename U, item_t<U> X>
using append_t = typename append<U, X>::type;

template <typename U, item_t<U> X>
struct prepend; // never reach this case

template <typename T, T... Xs, T X>
struct prepend<list<T, Xs...>, X>
{
  using type = list<T, X, Xs...>;
};

template <typename U, item_t<U> X>
using prepend_t = typename prepend<U, X>::type;

template <typename U, typename V>
struct concat; // never reach this case

template <typename T, T... Xs, T... Ys>
struct concat<list<T, Xs...>, list<T, Ys...>>
{
  using type = list<T, Xs..., Ys...>;
};

template <typename U, typename V>
using concat_t = typename concat<U, V>::type;

template <typename U>
struct split; // never reach this case

template <typename T, T X, T... Xs>
struct split<list<T, X, Xs...>>
{
  static constexpr T head = X;

  using tail = list<T, Xs...>;
};

template <typename U>
using tail_t = typename split<U>::tail;

template <typename U>
constexpr item_t<U> head()
{
  return split<U>::head;
}

template <typename U>
struct revert;

template <typename U>
using revert_t = typename revert<U>::type;

template <typename U>
struct revert
{
  using type = append_t<
    revert_t<tail_t<U>>,
    head<U>()
  >;
};

template <typename T>
struct revert<list<T>>
{
  using type = list<T>;
};

template <typename U>
struct for_each;

template <typename T, T... Xs>
struct for_each<list<T, Xs...>>
{
  template <typename F>
  static void apply(F f)
  {
    int dummy[] = { (f(Xs), 0)... };
  }
};

template <typename U>
struct foldl
{
  template <typename S, typename F>
  static constexpr auto with(S s, F f)
  {
    return foldl<tail_t<U>>::with(f(s, head<U>()), f);
  }
};

template <typename T>
struct foldl<list<T>>
{
  template <typename S, typename F>
  static constexpr auto with(S s, F)
  {
    return s;
  }
};

template <typename U>
struct foldr
{
  template <typename S, typename F>
  static constexpr auto with(S s, F f)
  {
    return f(head<U>(), foldr<tail_t<U>>::with(s, f));
  }
};

template <typename T>
struct foldr<list<T>>
{
  template <typename S, typename F>
  static constexpr auto with(S s, F)
  {
    return s;
  }
};

} // namespace compile_time_list

