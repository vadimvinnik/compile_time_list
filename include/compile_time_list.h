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
constexpr auto size_v = U::size;

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
struct uncons; // never reach this case

template <typename T, T X, T... Xs>
struct uncons<list<T, X, Xs...>>
{
  static constexpr T head = X;

  using tail = list<T, Xs...>;
};

template <typename U>
using tail_t = typename uncons<U>::tail;

template <typename U>
constexpr item_t<U> head_v = uncons<U>::head;

template <typename U>
struct revert; // forward, defined further

template <typename U>
using revert_t = typename revert<U>::type;

template <typename U>
struct revert
{
  using type = append_t<
    revert_t<tail_t<U>>,
    head_v<U>
  >;
};

template <typename T>
struct revert<list<T>>
{
  using type = list<T>;
};

template <
  typename U,
  typename V,
  template <auto> typename P>
struct span_loop; // forward, defined further

template <
  typename U,
  typename V,
  template <auto> typename P,
  auto X,
  bool B>
struct span_loop_if // only reach this case when B is true
{
private:
  using step = span_loop<U, append_t<V, X>, P>;

public:
  using left = typename step::left;
  using right = typename step::right;
};

template <
  typename U,
  typename V,
  template <auto> typename P,
  auto X>
struct span_loop_if<U, V, P, X, false>
{
  using left = V;
  using right = prepend_t<U, X>;
};

template <
  typename U,
  typename V,
  template <auto> typename P>
struct span_loop // only reached if U is not empty
{
private:
  using step = span_loop_if<
    tail_t<U>,
    V,
    P,
    head_v<U>,
    P<head_v<U>>::value>;

public:
  using left = typename step::left;
  using right = typename step::right;
};

template <
  typename T,
  typename V,
  template <auto> typename P>
struct span_loop<list<T>, V, P>
{
  using left = V;
  using right = list<T>;
};

template <
  typename U,
  template <auto> typename P,
  typename T = item_t<U>>
using span = span_loop<U, list<T>, P>;

template <
  typename U,
  typename V,
  typename W,
  template <auto> typename P>
struct partition_loop; // forward, defined further

template <
  typename U,
  typename V,
  typename W,
  template <auto> typename P,
  auto X,
  bool B>
struct partition_loop_if // only reach this case when B is true
{
  using step = partition_loop<U, append_t<V, X>, W, P>;
};

template <
  typename U,
  typename V,
  typename W,
  template <auto> typename P,
  auto X>
struct partition_loop_if<U, V, W, P, X, false>
{
  using step = partition_loop<U, V, append_t<W, X>, P>;
};

template <
  typename U,
  typename V,
  typename W,
  template <auto> typename P>
struct partition_loop // only reached if U is not empty
{
private:
  using step = typename partition_loop_if<
    tail_t<U>,
    V,
    W,
    P,
    head_v<U>,
    P<head_v<U>>::value
  >::step;

public:
  using left = typename step::left;
  using right = typename step::right;
};

template <
  typename T,
  typename V,
  typename W,
  template <auto> typename P>
struct partition_loop<list<T>, V, W, P>
{
  using left = V;
  using right = W;
};

template <
  typename U,
  template <auto> typename P,
  typename T = item_t<U>>
using partition = partition_loop<U, list<T>, list<T>, P>;

template <typename U, template <auto> typename F>
struct fmap; // never reach this case

template <typename T, T... Xs, template <auto> typename F>
struct fmap<list<T, Xs...>, F>
{
  using result = list<T, F<Xs>::value...>;
};

template <typename U, template <auto> typename F>
using fmap_t = typename fmap<U, F>::result;

template <typename U>
struct for_each; // never reach this case

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
    return foldl<tail_t<U>>::with(f(s, head_v<U>), f);
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
    return f(head_v<U>, foldr<tail_t<U>>::with(s, f));
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

