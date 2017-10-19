#include "catch/catch.hpp"
#include "io/memory_stream.hh"

#include <array>

using namespace io;
using namespace std;

namespace
{
TEST_CASE("working with an empty MemoryStream")
{
  array<char, 16> buff{};
  MemoryStream<decltype(buff)> ms(buff, buff.size(), 0 /* size */, 0 /* pos */);

  char c = '\0';

  REQUIRE(!ms.eof());

  SECTION("read fails on empty stream")
  {
    REQUIRE(!ms.read(c));
  }
  SECTION("write bytes and read them back")
  {
    char bytes[] = "Hello, world";
    for (auto c : bytes)
      ms.write(c);
    ms.rewind();
    size_t i = 0;
    while (ms.read(c))
      REQUIRE(c == bytes[i++]);
    REQUIRE(i == sizeof(bytes));
  }
}

TEST_CASE("read from a preloaded stream")
{
  char buff[] = "Hello, world";
  MemoryStream<decltype(buff)> ms(buff, sizeof(buff), sizeof(buff) - 1 /* size */, 0 /* pos */);

  char c = '\0';

  REQUIRE(!ms.eof());

  SECTION("write bytes and read them back")
  {
    char otherBuff[] = "Hello, world";
    size_t i = 0;
    while (ms.read(c))
      REQUIRE(c == otherBuff[i++]);
    REQUIRE(i == sizeof(buff) - 1);
  }
}

TEST_CASE("eof should raise only after a reading attempt")
{
  char buff[] = {'c'};
  MemoryStream<decltype(buff)> ms(buff, sizeof(buff), 1 /* size */);

  char c = '\0';

  REQUIRE(!ms.eof());
  REQUIRE(ms.read(c));
  REQUIRE(!ms.eof());
  REQUIRE(!ms.read(c));
}

TEST_CASE("override existing data")
{
  char buff[] = "Hello, world";
  MemoryStream<decltype(buff)> ms(buff, sizeof(buff), sizeof(buff) - 1 /* size */, 0 /* pos */);

  for (auto const c : {'B', 'y', '-', 'b', 'y'})
    ms.write(c);

  REQUIRE(string(buff) == "By-by, world");
}
}  // namespace
