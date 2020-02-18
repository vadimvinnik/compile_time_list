#include "compile_time_list.h"

#include <algorithm>
#include <functional>
#include <type_traits>

using namespace compile_time_list;

namespace aux
{

template <typename T, T L>
struct bind_1st
{
  template <T R>
  struct is_not_equal_to : std::bool_constant<(L != R)> {};

  template <T R>
  struct is_divisor_of : std::bool_constant<(R % L == 0)> {};

  template <T R>
  struct multiply_by : std::integral_constant<int, R * L> {};
};

template <typename S, typename T>
struct binary
{
  template <S X, T Y>
  struct plus
  {
    static constexpr auto value = X + Y;
  };

  template <S L, T R>
  struct less_than : std::bool_constant<(L < R)> {};
};

template <unsigned N, unsigned S, typename T>
struct iota_step;

template <unsigned N, unsigned S, unsigned I>
struct iota_step<N, S, std::integral_constant<unsigned, I>>
{
  using result = std::integral_constant<unsigned, std::min(N, I+S)>;
};

template <unsigned N, unsigned S>
struct iota_step<N, S, std::integral_constant<unsigned, N>>
{};

template <unsigned N, unsigned S>
struct iota
{
  template <typename T>
  using step = iota_step<N, S, T>;
};

} // namespace aux

namespace append_prepend_test
{

using t_1 = list<int, 0>;
using t_2 = prepend_t<t_1, 1>;
using t_3 = append_t<t_2, 2>;
using t_4 = prepend_t<t_3, 3>;
using t_5 = append_t<t_4, 4>;

using expexted = list<int, 3, 1, 0, 2, 4>;

static_assert(std::is_same<t_5, expexted>::value);

} // namespace append_prepend_test

namespace revert_test
{

using arg = list<int, 0, 1, 2, 3>;

using result = revert_t<arg>;

using expexted = list<int, 3, 2, 1, 0>;

static_assert(std::is_same<result, expexted>::value);

} // namespace revert_test

namespace concat_test
{

using arg_1 = list<int, 0, 1, 2>;
using arg_2 = list<int, 3, 4, 5, 6>;

using result = concat_t<arg_1, arg_2>;

using expexted = list<int, 0, 1, 2, 3, 4, 5, 6>;

static_assert(std::is_same<result, expexted>::value);

} // namespace concat_test

namespace concat_empty_test
{

using arg = list<int, 0, 1, 2>;
using empty = list<int>;

using result_1 = concat_t<arg, empty>;
using result_2 = concat_t<empty, arg>;
using result_3 = concat_t<empty, empty>;

static_assert(std::is_same<result_1, arg>::value);
static_assert(std::is_same<result_2, arg>::value);
static_assert(std::is_same<result_3, empty>::value);

} // namespace concat_empty_test

namespace span_test
{

using arg = list<int, 0, 9, 1, 8, 2, 7, 3, 6, 4, 5>;
using result = span<arg, aux::bind_1st<int, 7>::is_not_equal_to>;

using expexted_left = list<int, 0, 9, 1, 8, 2>;
using expexted_right = list<int, 7, 3, 6, 4, 5>;

static_assert(std::is_same<result::left, expexted_left>::value);
static_assert(std::is_same<result::right, expexted_right>::value);

} // namespace span_test

namespace span_empty_test
{

using arg = list<int, 0, 9, 1, 8, 2, 7, 3, 6, 4, 5>;
using result_1 = span<arg, aux::bind_1st<int, 0>::is_not_equal_to>;
using result_2 = span<arg, aux::bind_1st<int, 10>::is_not_equal_to>;

static_assert(std::is_same<result_1::left, list<int>>::value);
static_assert(std::is_same<result_1::right, arg>::value);

static_assert(std::is_same<result_2::left, arg>::value);
static_assert(std::is_same<result_2::right, list<int>>::value);

} // namespace span_empty_test

namespace partition_test
{

using arg = list<int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9>;
using result = partition<arg, aux::bind_1st<int, 2>::is_divisor_of>;

using expexted_left = list<int, 0, 2, 4, 6, 8>;
using expexted_right = list<int, 1, 3, 5, 7, 9>;

static_assert(std::is_same<result::left, expexted_left>::value);
static_assert(std::is_same<result::right, expexted_right>::value);

} // namespace partition_test

namespace fmap_test
{

using arg = list<int, 3, 2, 1, 0>;
using result_1 = fmap_t<arg, aux::bind_1st<int, 3>::multiply_by>;
using result_2 = fmap_t<arg, aux::bind_1st<int, 2>::is_divisor_of>;

using expexted_1 = list<int, 9, 6, 3, 0>;
using expexted_2 = list<int, false, true, false, true>;

static_assert(std::is_same<result_1, expexted_1>::value);
static_assert(std::is_same<result_2, expexted_2>::value);

} // namespace fmap_test

namespace zip_with_test
{

using arg1 = list<unsigned, 0x01, 0x02, 0x03, 0x04, 0x05>;
using arg2 = list<unsigned, 0xA0, 0xB0, 0xC0>;

using result = zip_with_t<
  arg1,
  arg2,
  unsigned,
  aux::binary<unsigned, unsigned>::plus>;

using expexted = list<unsigned, 0xA1, 0xB2, 0xC3>;

static_assert(std::is_same<result, expexted>::value);

} // namespace zip_with_test

namespace sort_test
{

using arg = list<int, 0, 9, 1, 8, 2, 7, 3, 6, 4, 5>;
using result = sort_t<arg, aux::binary<int, int>::less_than>;
using expexted = list<int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9>;

static_assert(std::is_same<result, expexted>::value);

}

namespace fold_plus_test
{

using arg = list<int, 0, 1, 2, 4, 8>;
constexpr int result_1 = foldl_v<arg, 0, aux::binary<int, int>::plus>;
constexpr int result_2 = foldr_v<arg, 0, aux::binary<int, int>::plus>;

constexpr int expexted = 0 + 1 + 2 + 4 + 8;

static_assert(result_1 == expexted);
static_assert(result_2 == expexted);

} // namespace foldl_plus_test

namespace unfoldr_iota_test
{

using result = unfoldr_t<
  unsigned,
  std::integral_constant<unsigned, 0>,
  aux::iota<10, 1>::step>;

using expexted = list<unsigned, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9>;

static_assert(std::is_same<result, expexted>::value);

} // namespace unfoldr_iota_test

namespace unfoldr_fibonacci_test
{

template <unsigned A, unsigned B, unsigned K>
struct fibonacci_state
{
  static constexpr unsigned value = A;
};

template <typename T>
struct fibonacci_step;

template <unsigned A, unsigned B, unsigned K>
struct fibonacci_step<fibonacci_state<A, B, K>>
{
  using result = fibonacci_state<B, A+B, K-1>;
};

template <unsigned A, unsigned B>
struct fibonacci_step<fibonacci_state<A, B, 0>>
{};

using result = unfoldr_t<
  unsigned,
  fibonacci_state<1, 1, 10>,
  fibonacci_step>;

using expexted = list<unsigned, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55>;

static_assert(std::is_same<result, expexted>::value);

} // namespace unfoldr_fibonacci_test

namespace unfoldr_primes_test
{

template <typename C>
struct eratosthenes_state
{
  static constexpr unsigned value = head_v<C>;
};

template <typename C>
struct eratosthenes_step;

template <typename U>
struct eratosthenes_step<eratosthenes_state<U>>
{
private:
  template <unsigned X>
  struct predicate : std::bool_constant<(X % head_v<U> == 0)> {};

  using filtered = typename partition<U, predicate>::right;

public:
  using result = eratosthenes_state<filtered>;
};

template <>
struct eratosthenes_step<eratosthenes_state<list<unsigned>>> {};

using odds_after_2 = unfoldr_t<
  unsigned,
  std::integral_constant<unsigned, 3>,
  aux::iota<100, 2>::step>;

using initial = prepend_t<odds_after_2, 2>;

using result = unfoldr_t<
  unsigned,
  eratosthenes_state<initial>,
  eratosthenes_step>;

using expexted = list<
  unsigned,
   2,  3,  5,  7, 11,
  13, 17, 19, 23, 29,
  31, 37, 41, 43, 47,
  53, 59, 61, 67, 71,
  73, 79, 83, 89, 97>;

static_assert(std::is_same<result, expexted>::value);

} // namespace unfoldr_primes_test

