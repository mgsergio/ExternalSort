#include "catch/catch.hpp"
#include "io/file_stream.hh"

#include <cstdio>
#include <cstring>
// #include <array>

using namespace io;
using namespace std;

namespace
{
class FileGuard
{
public:
  FileGuard(string const & fileName, string const mode)
    : m_fileName(fileName)
    , m_mode(mode)
  {
    m_file = fopen(m_fileName.data(), m_mode.data());
  }

  ~FileGuard()
  {
    if (m_file)
    {
      fclose(m_file);
      remove(m_fileName.data());
    }
  }

  string const & getFileName() const { return m_fileName; }
  string const & getMode() const { return m_mode; }
  FILE * getFile() { return m_file; }

private:
  string m_fileName;
  string m_mode;
  FILE * m_file = nullptr;
};

char const testFileName[] = ".FileStreamTest";
char const openMode[] = "w+x";

TEST_CASE("[FileStream] working with an empty Stream")
{
  FileGuard fg(testFileName, openMode);
  FileStream fs(fg.getFile(), fg.getMode());

  char c = '\0';

  REQUIRE(!fs.eof());

  SECTION("read fails on empty stream")
  {
    REQUIRE(!fs.read(c));
  }
  SECTION("write bytes and read them back")
  {
    char bytes[] = "Hello, world";
    for (auto c : bytes)
      fs.write(c);
    fs.rewind();
    size_t i = 0;
    while (fs.read(c))
      REQUIRE(c == bytes[i++]);
    REQUIRE(i == sizeof(bytes));
  }
  SECTION("write a chunk of butes")
  {
    char const input[] = "Hello, world";
    fs.write(input, sizeof(input));
    fs.rewind();

    char output[sizeof(input)]{};
    fs.read(output, sizeof(output));

    REQUIRE(strcmp(input, output) == 0);
  }
}

TEST_CASE("[FileStream] read from a preloaded stream")
{
  char buff[] = "Hello, world";

  FileGuard fg(testFileName, openMode);
  fwrite(buff, sizeof(char), sizeof(buff) - 1, fg.getFile());
  rewind(fg.getFile());

  FileStream fs(fg.getFile(), fg.getMode());

  char c = '\0';

  REQUIRE(!fs.eof());

  SECTION("read bytes one by one")
  {
    char otherBuff[] = "Hello, world";
    size_t i = 0;
    while (fs.read(c))
      REQUIRE(c == otherBuff[i++]);
    REQUIRE(i == sizeof(buff) - 1);
  }
  SECTION("read a chunk of bytes")
  {
    char output[16]{};
    fs.read(output, sizeof(buff));
    REQUIRE(strcmp(output, buff) == 0);
  }
}

TEST_CASE("[FileStream]eof should raise only after a reading attempt")
{
  FileGuard fg(testFileName, openMode);
  fputc('c', fg.getFile());
  rewind(fg.getFile());

  FileStream fs(fg.getFile(), fg.getMode());
  char buff[] = {'c'};

  char c = '\0';

  REQUIRE(!fs.eof());
  REQUIRE(fs.read(c));
  REQUIRE(!fs.eof());
  REQUIRE(!fs.read(c));
}

TEST_CASE("[FileStream] override existing data")
{
  char buff[] = "Hello, world";

  FileGuard fg(testFileName, openMode);
  fwrite(buff, sizeof(char), sizeof(buff) - 1, fg.getFile());
  rewind(fg.getFile());

  FileStream fs(fg.getFile(), fg.getMode());

  for (auto const c : {'B', 'y', '-', 'b', 'y'})
    fs.write(c);
  rewind(fg.getFile());

  fread(buff, sizeof(char), sizeof(buff) - 1, fg.getFile());
  REQUIRE(string(buff) == "By-by, world");
}
}  // namespace
