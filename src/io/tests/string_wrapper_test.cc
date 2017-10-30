#include "catch/catch.hpp"
#include "io/memory_stream.hh"
#include "io/string_wrapper.hh"

#include "helpers.hh"

#include <array>
#include <initializer_list>
#include <iterator>

using namespace io;
using namespace std;

namespace
{
TEST_CASE("[StringWrapper] smoke")
{
  array<char, 128> buff{};
  MemoryStream<decltype(buff)> ms(buff, buff.size(), 0 /* size */);

  StringWrapper sw(ms);

  initializer_list<string> strings = {
    "Hello",
    "World",
    "!",
    "Some",
    "",
    "",
    "more",
    "strings"
  };

  string s;

  SECTION("read fails on empty stream")
  {
    REQUIRE(!sw.read(s));
  }
  SECTION("writing strings and read them back")
  {
    tests::writeStringToSink(sw, strings);

    sw.rewind();
    auto it = begin(strings);
    while (sw.read(s))
    {
      REQUIRE(s == *it++);
    }
    REQUIRE(distance(begin(strings), it) == strings.size());
  }
  SECTION("read empty string")
  {
    sw.write("");
    sw.rewind();

    REQUIRE(sw.read(s));
    REQUIRE(s.empty());
  }
  SECTION("read fails on stream with no \n at the end")
  {
    for (size_t i = 0; i < buff.size(); ++i)
      ms.write('a');
    sw.rewind();
    REQUIRE(!sw.read(s));
  }
}
}  // namespace
