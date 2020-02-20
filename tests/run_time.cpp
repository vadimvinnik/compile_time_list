#include "core.h"

#include <catch.hpp>

using namespace compile_time_list;

TEST_CASE("for_each works")
{
  unsigned long s = 0l;

  using arg = list<unsigned, 0xC, 0x0, 0xD, 0xE, 0x9, 0x0, 0x0, 0xD>;

  for_each<arg>::apply([&s](unsigned x) { s = (s << 4) | x; });

  REQUIRE(s == 0xC0DE'900D);
}

