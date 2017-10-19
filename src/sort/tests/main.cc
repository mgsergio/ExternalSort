#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include "io/memory_stream.hh"
#include "io/string_wrapper.hh"
#include "sort/sort.hh"

#include <vector>

using namespace io;
using namespace sort;
using namespace std;

namespace
{
#define CHECK_SORT_ON_INPUT                                      \
  do {                                                           \
    for (auto const & s : input)                                 \
      in.write(s);                                               \
    in.rewind();                                                 \
                                                                 \
    mergeSort(in, out, tmp);                                     \
                                                                 \
    out.rewind();                                                \
                                                                 \
    string s;                                                    \
    for (size_t i = 0; out.read(s); ++i)                         \
      REQUIRE(s == expected[i]);                                 \
  } while(0)


TEST_CASE("merge sort tests")
{
  vector<char> inBuff(512);
  vector<char> outBuff(inBuff.size());
  vector<char> tmpBuff(inBuff.size());

  MemoryStream<vector<char>> memIn(inBuff, inBuff.size(), inBuff.size() - 1);
  MemoryStream<vector<char>> memOut(outBuff, inBuff.size(), 0);
  MemoryStream<vector<char>> memTmp(tmpBuff, inBuff.size(), 0);

  StringWrapper in(memIn);
  StringWrapper out(memOut);
  StringWrapper tmp(memTmp);

  SECTION("single string")
  {
    vector<string> const input{"1"};
    vector<string> const expected{"1"};
    CHECK_SORT_ON_INPUT;
  }
  SECTION("trivial")
  {
    vector<string> const input{"1", "2", "3"};
    vector<string> const expected{"1", "2", "3"};
    CHECK_SORT_ON_INPUT;
  }
  SECTION("trivial reversed")
  {
    vector<string> const input{"3", "2", "1"};
    vector<string> const expected{"1", "2", "3"};
    CHECK_SORT_ON_INPUT;
  }
  SECTION("more complex data")
  {
    vector<string> const input{
      "CvcAfERzdhUWg",
      "bg2iXCtPJS0bJA",
      "UJQ6CHBLmV",
      "PDrkVO6aNBHqmf",
      "BSgOOl5e9em",
      "xdA7CnK2QJKvE5",
      "kaQqYeIlgZ"
    };
    vector<string> const expected{
      "BSgOOl5e9em",
      "CvcAfERzdhUWg",
      "PDrkVO6aNBHqmf",
      "UJQ6CHBLmV",
      "bg2iXCtPJS0bJA",
      "kaQqYeIlgZ",
      "xdA7CnK2QJKvE5"
    };
    CHECK_SORT_ON_INPUT;
  }
  SECTION("and even more complex data")
  {
    vector<string> const input{
      "gKLR1gcaxoz",
      "4PPU8boqhhwwVToRFirMbt0XvU",
      "HQxxdJ4KUkemLb8LHDqAUuF",
      "OMAEprPV3IfPwleRv7",
      "BH0IBwwnE3FWXwzZxr8AezSbnA",
      "A2YccfageTIAP6q05bmpNYi9a0",
      "y66DxMllS7L4BLwcq1p",
      "tI0F1W8umY3VhMD8D66th",
      "McE1I7769i4MmQ",
      "ok4MaXDIRcNKMu3Me",
      "upBmjqexutnSqMMxj696",
      "PQYB0US1YXU1SrwP8QR2tuD6sJOC2",
      "uknOHltimTj04",
      "xK61oWPaYTdJ6",
      "ohCnYUVUj1uPgZVGFW",
      "4TfKBXIO0UwFGFV44HgVblkcDAE",
      "MPOThAaqvEqtzEQPclaOmPhNXjFb",
      "ZVuViMTIewLSzwm",
      "PdIahl7pTDwhZWXeOMjs",
      "idHrqw6bFUq",
      "xEbWvL8PyZ4y",
      "h5Vm9sr23LrgqkKbR0",
      "GfiaZmFPoEN53bf",
      "96KBYfoWXPhA1",
      "r1BqEofDwMTdNbPQiplPft"
    };
    vector<string> const expected{
      "4PPU8boqhhwwVToRFirMbt0XvU",
      "4TfKBXIO0UwFGFV44HgVblkcDAE",
      "96KBYfoWXPhA1",
      "A2YccfageTIAP6q05bmpNYi9a0",
      "BH0IBwwnE3FWXwzZxr8AezSbnA",
      "GfiaZmFPoEN53bf",
      "HQxxdJ4KUkemLb8LHDqAUuF",
      "MPOThAaqvEqtzEQPclaOmPhNXjFb",
      "McE1I7769i4MmQ",
      "OMAEprPV3IfPwleRv7",
      "PQYB0US1YXU1SrwP8QR2tuD6sJOC2",
      "PdIahl7pTDwhZWXeOMjs",
      "ZVuViMTIewLSzwm",
      "gKLR1gcaxoz",
      "h5Vm9sr23LrgqkKbR0",
      "idHrqw6bFUq",
      "ohCnYUVUj1uPgZVGFW",
      "ok4MaXDIRcNKMu3Me",
      "r1BqEofDwMTdNbPQiplPft",
      "tI0F1W8umY3VhMD8D66th",
      "uknOHltimTj04",
      "upBmjqexutnSqMMxj696",
      "xEbWvL8PyZ4y",
      "xK61oWPaYTdJ6",
      "y66DxMllS7L4BLwcq1p"
    };
    CHECK_SORT_ON_INPUT;
  }
  SECTION("empty string")
  {
    vector<string> const input{"", "2", "1"};
    vector<string> const expected{"", "1", "2"};
    CHECK_SORT_ON_INPUT;
  }
}
}  // namespace
