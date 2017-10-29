#pragma once

#include "stream.hh"
#include "string_view.hh"

#include <vector>

namespace io
{
class BufferedStringWrapper
{
public:
  using value_type = StringView;

  BufferedStringWrapper(IStream & stream, size_t buff_size)
    : m_stream(stream)
    , m_buff(buff_size)
  {
    init();
  }

  ~BufferedStringWrapper() { flush(); }

  bool read(StringView & s);
  void write(StringView const & s);
  void flush();

  bool eof() const { return m_stream.eof(); }

  void rewind();
  void truncate();

private:
  void init();
  void fetch();

  bool empty() const;

  IStream & m_stream;
  std::vector<char> m_buff;
  size_t m_pos;
  size_t m_end;
};
}  // namespace io
