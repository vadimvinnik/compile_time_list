#include "compile_time_list.h"

#include <catch.hpp>

#include <functional>
#include <string>
#include <type_traits>

using namespace compile_time_list;

using t_1 = list<int, 0>;
using t_2 = prepend_t<t_1, 1>;
using t_3 = append_t<t_2, 2>;
using t_4 = prepend_t<t_3, 3>;
using t_5 = append_t<t_4, 4>;

static_assert(std::is_same<t_5, list<int, 3, 1, 0, 2, 4>>::value);

using u_1 = list<int, 8, 7, 6, 5>;
using u_2 = revert_t<u_1>;

static_assert(std::is_same<u_2, list<int, 5, 6, 7, 8>>::value);

using v = concat_t<t_5, u_2>;

static_assert(std::is_same<v, list<int, 3, 1, 0, 2, 4, 5, 6, 7, 8>>::value);

static_assert(foldl<v>::with(0, std::plus<int>()) == 36);

template <typename T, T L, T R>
struct is_less_t : std::bool_constant<(L < R)>
{};

template <int X>
struct is_less_than_5_t: is_less_t<int, X, 5>
{};

using s = span<v, is_less_than_5_t>;

static_assert(std::is_same<s::left, list<int, 3, 1, 0, 2, 4>>::value);
static_assert(std::is_same<s::right, list<int, 5, 6, 7, 8>>::value);

template <int X>
struct is_odd_t: std::bool_constant<(X % 2 != 0)>
{};

using p = partition<v, is_odd_t>;

static_assert(std::is_same<p::left, list<int, 3, 1, 5, 7>>::value);
static_assert(std::is_same<p::right, list<int, 0, 2, 4, 6, 8>>::value);

TEST_CASE("for_each works")
{
  std::string s;
  for_each<v>::apply([&s](int x) { s += std::to_string(x); });

  REQUIRE(s == "310245678");
}

