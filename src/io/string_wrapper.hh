#pragma once

#include "stream.hh"

#include <string>

namespace io
{
class StringWrapper
{
public:
  using value_type = std::string;

  StringWrapper(IStream & stream)
    : m_stream(stream)
  {
  }

  bool read(std::string & s);
  void write(std::string const & s);

  bool eof() const { return m_stream.eof(); }

  void rewind() { m_stream.rewind(); }
  void truncate() { m_stream.truncate(); }

private:
  IStream & m_stream;
};
}  // namespace io
