#include "catch/catch.hpp"
#include "io/string_view.hh"

using namespace io;

namespace
{
TEST_CASE("[StringView] smoke")
{
  char buff[] = "Hello, world!";
  size_t size = sizeof(buff) - 1;

  StringView sv(buff, size);

  SECTION("size")
  {
    REQUIRE(sv.size() == size);
  }
  SECTION("check content")
  {
    size_t i = 0;
    for (; i < sv.size(); ++i)
      REQUIRE(sv[i] == buff[i]);
    REQUIRE(sv[i - 1] == '!');
  }
  SECTION("operators")
  {
    {
      StringView osv(buff, size);

      REQUIRE(!(sv < osv));
      REQUIRE(sv <= osv);
      REQUIRE(!(sv > osv));
      REQUIRE(sv >= osv);
      REQUIRE(sv == osv);
      REQUIRE(!(sv != osv));
    }
    {
      char obuff[] = "Hello";
      StringView osv(obuff, size);

      REQUIRE(!(sv < osv));
      REQUIRE(!(sv <= osv));
      REQUIRE(sv > osv);
      REQUIRE(sv >= osv);
      REQUIRE(!(sv == osv));
      REQUIRE(sv != osv);
    }
    {
      char obuff[] = "Absolutely";
      StringView osv(obuff, size);

      REQUIRE(!(sv < osv));
      REQUIRE(!(sv <= osv));
      REQUIRE(sv > osv);
      REQUIRE(sv >= osv);
      REQUIRE(!(sv == osv));
      REQUIRE(sv != osv);
    }
  }
}  // TEST_CASE
}  // namespace
