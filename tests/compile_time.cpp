#include "compile_time_list.h"

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

namespace foldl_plus_test
{

using arg = list<int, 0, 1, 2, 4, 8>;
constexpr int result = foldl<arg>::with(0, std::plus<int>());

constexpr int expexted = 0 + 1 + 2 + 4 + 8;

static_assert(result == expexted);

} // namespace foldl_plus_test

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

