#pragma once

#include "stream.hh"

#include <cstdio>
#include <string>

namespace io
{
class FileStream : public IStream
{
public:
  FileStream(FILE * file, std::string const & mode);
  FileStream(std::string const & fileName, std::string const & mode);

  ~FileStream();

  bool eof() const;

  void rewind();
  void truncate();

  bool read(char & c);
  size_t read(char buff[], size_t size);

  void write(char c);
  size_t write(char const buff[], size_t size);

private:
  FILE * m_file = nullptr;
  std::string m_mode;
  bool m_own = false;
};
}  // namespace io
