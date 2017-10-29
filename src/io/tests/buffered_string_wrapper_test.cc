#include "catch/catch.hpp"
#include "io/buffered_string_wrapper.hh"
#include "io/memory_stream.hh"
#include "io/string_view.hh"

#include "helpers.hh"

#include <array>
#include <initializer_list>
#include <iterator>

using namespace io;
using namespace std;

namespace io
{
bool operator==(StringView const & sv, string const & s)
{
  return sv == StringView(s.data(), s.size());
}

ostream & operator<<(ostream & os, StringView const & s )
{
  for (size_t i = 0; i < s.size(); ++i)
    os << s[i];
  return os;
}
}  // namespace io

namespace
{
TEST_CASE("[BufferedStringWrapper] smoke")
{
  array<char, 128> buff{};
  MemoryStream<decltype(buff)> ms(buff, buff.size(), 0 /* size */);

  size_t const bswBuffSize = 8;
  BufferedStringWrapper bsw(ms, bswBuffSize);

  initializer_list<string> strings = {
    "Hello",
    "World",
    "!",
    "Some",
    "more",
    "strings"
  };

  StringView s;

  SECTION("read fails on empty stream")
  {
    REQUIRE(!bsw.read(s));
  }
  SECTION("writing strings and read them back")
  {
    tests::writeStringToSink(bsw, strings);

    bsw.flush();
    // printf("What have we here?\n");
    // for (size_t i = 0; i < buff.size() && buff[i]; ++i)
    //   putc(buff[i], stdout);
    // putc('\n', stdout);
    bsw.rewind();
    auto it = begin(strings);
    while (bsw.read(s))
    {
      INFO("Iteration");
      REQUIRE(s == *it++);
    }
    REQUIRE(distance(begin(strings), it) == strings.size());
  }
  SECTION("read empty string")
  {
    bsw.write(StringView());
    bsw.flush();
    bsw.rewind();

    REQUIRE(bsw.read(s));
    REQUIRE(s.empty());
  }
  SECTION("read fails on stream with no \n at the end")
  {
    for (size_t i = 0; i < buff.size(); ++i)
      ms.write('a');
    bsw.flush();
    bsw.rewind();
    REQUIRE(!bsw.read(s));
  }
}
}  // namespace
