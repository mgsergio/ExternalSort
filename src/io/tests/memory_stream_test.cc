#include "catch/catch.hpp"
#include "io/memory_stream.hh"

#include <array>
#include <cstring>

using namespace io;
using namespace std;

namespace
{
TEST_CASE("[MemoryStream] working with an empty")
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
  SECTION("write a chunk of butes")
  {
    char const input[] = "Hello, world";
    ms.write(input, sizeof(input));
    REQUIRE(strcmp(input, buff.data()) == 0);
  }
}

TEST_CASE("[MemoryStream] read from a preloaded stream")
{
  char buff[] = "Hello, world";
  MemoryStream<decltype(buff)> ms(buff, sizeof(buff), sizeof(buff) - 1 /* size */, 0 /* pos */);

  char c = '\0';

  REQUIRE(!ms.eof());

  SECTION("read bytes one by one")
  {
    char otherBuff[] = "Hello, world";
    size_t i = 0;
    while (ms.read(c))
      REQUIRE(c == otherBuff[i++]);
    REQUIRE(i == sizeof(buff) - 1);
  }
  SECTION("read a chunk of bytes")
  {
    array<char, 16> output;
    auto const bytesRead = ms.read(output.data(), sizeof(buff));
    REQUIRE(bytesRead == sizeof(buff) - 1);
    REQUIRE(strcmp(output.data(), buff) == 0);
  }
}

TEST_CASE("[MemoryStream] eof should raise only after a reading attempt")
{
  char buff[] = {'c'};
  MemoryStream<decltype(buff)> ms(buff, sizeof(buff), 1 /* size */);

  char c = '\0';

  REQUIRE(!ms.eof());
  REQUIRE(ms.read(c));
  REQUIRE(!ms.eof());
  REQUIRE(!ms.read(c));
}

TEST_CASE("[MemoryStream] override existing data")
{
  char buff[] = "Hello, world";
  MemoryStream<decltype(buff)> ms(buff, sizeof(buff), sizeof(buff) - 1 /* size */, 0 /* pos */);

  for (auto const c : {'B', 'y', '-', 'b', 'y'})
    ms.write(c);

  REQUIRE(string(buff) == "By-by, world");
}
}  // namespace
