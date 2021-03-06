#include "core.h"

#include "primes.h"
#include "fibonacci.h"
#include "iota.h"

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

} // namespace aux

namespace size_empty_test
{

using arg = list<int>;

static_assert(size_v<arg> == 0);
static_assert(is_empty_v<arg>);

} // namespace size_empty_test

namespace size_non_empty_test
{

using arg = list<int, 0, 1, 2>;

static_assert(size_v<arg> == 3);
static_assert(!is_empty_v<arg>);

} // namespace size_non_empty_test

namespace append_prepend_test
{

using t_1 = list<int, 0>;
using t_2 = prepend_t<t_1, 1>;
using t_3 = append_t<t_2, 2>;
using t_4 = prepend_t<t_3, 3>;
using t_5 = append_t<t_4, 4>;

using expected = list<int, 3, 1, 0, 2, 4>;

static_assert(std::is_same<t_5, expected>::value);

} // namespace append_prepend_test

namespace revert_test
{

using arg = list<int, 0, 1, 2, 3>;

using result = revert_t<arg>;

using expected = list<int, 3, 2, 1, 0>;

static_assert(std::is_same<result, expected>::value);

} // namespace revert_test

namespace concat_test
{

using arg_1 = list<int, 0, 1, 2>;
using arg_2 = list<int, 3, 4, 5, 6>;

using result = concat_t<arg_1, arg_2>;

using expected = list<int, 0, 1, 2, 3, 4, 5, 6>;

static_assert(std::is_same<result, expected>::value);

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

namespace uncons_test
{

using arg = list<int, 0, 1, 2>;
using expected_tail = list<int, 1, 2>;

static_assert(head_v<arg> == 0);
static_assert(std::is_same<tail_t<arg>, expected_tail>::value);

} // namespace uncons_test

namespace uncons_empty_tail_test
{

using arg = list<int, 0>;
using expected_tail = list<int>;

static_assert(head_v<arg> == 0);
static_assert(std::is_same<tail_t<arg>, expected_tail>::value);

} // namespace uncons_empty_tail_test

namespace unconsr_test
{

using arg = list<int, 0, 1, 2>;
using expected_init = list<int, 0, 1>;

static_assert(last_v<arg> == 2);
static_assert(std::is_same<init_t<arg>, expected_init>::value);

} // namespace unconsr_test

namespace unconsr_empty_init_test
{

using arg = list<int, 0>;
using expected_init = list<int>;

static_assert(last_v<arg> == 0);
static_assert(std::is_same<init_t<arg>, expected_init>::value);

} // namespace unconsr_empty_tail_test

namespace span_test
{

using arg = list<int, 0, 9, 1, 8, 2, 7, 3, 6, 4, 5>;
using result = span<arg, aux::bind_1st<int, 7>::is_not_equal_to>;

using expected_left = list<int, 0, 9, 1, 8, 2>;
using expected_right = list<int, 7, 3, 6, 4, 5>;

static_assert(std::is_same<result::left, expected_left>::value);
static_assert(std::is_same<result::right, expected_right>::value);

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

using expected_left = list<int, 0, 2, 4, 6, 8>;
using expected_right = list<int, 1, 3, 5, 7, 9>;

static_assert(std::is_same<result::left, expected_left>::value);
static_assert(std::is_same<result::right, expected_right>::value);

} // namespace partition_test

namespace fmap_test
{

using arg = list<int, 3, 2, 1, 0>;
using result_1 = fmap_t<arg, aux::bind_1st<int, 3>::multiply_by>;
using result_2 = fmap_t<arg, aux::bind_1st<int, 2>::is_divisor_of>;

using expected_1 = list<int, 9, 6, 3, 0>;
using expected_2 = list<int, false, true, false, true>;

static_assert(std::is_same<result_1, expected_1>::value);
static_assert(std::is_same<result_2, expected_2>::value);

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

using expected = list<unsigned, 0xA1, 0xB2, 0xC3>;

static_assert(std::is_same<result, expected>::value);

} // namespace zip_with_test

namespace sort_test
{

using arg = list<int, 0, 9, 1, 8, 2, 7, 3, 6, 4, 5>;
using result = sort_t<arg, aux::binary<int, int>::less_than>;
using expected = list<int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9>;

static_assert(std::is_same<result, expected>::value);

}

namespace fold_plus_test
{

using arg = list<int, 0, 1, 2, 4, 8>;
constexpr int result_1 = foldl_v<arg, 0, aux::binary<int, int>::plus>;
constexpr int result_2 = foldr_v<arg, 0, aux::binary<int, int>::plus>;

constexpr int expected = 0 + 1 + 2 + 4 + 8;

static_assert(result_1 == expected);
static_assert(result_2 == expected);

} // namespace foldl_plus_test

