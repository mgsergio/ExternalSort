#pragma once

#include "stream.hh"

#include <algorithm>
#include <cassert>

namespace io
{
template <typename Buff>
class MemoryStream : public IStream
{
public:
  MemoryStream(Buff & buff, size_t cap, size_t size, size_t pos = 0)
    : m_pos(pos)
    , m_size(size)
    , m_cap(cap)
    , m_buff(buff)
  {
    assert(m_pos < cap);
    assert(m_pos <= size);
  }

  bool read(char & c) override
  {
    if (eof())
      return false;
    if (m_pos < m_size)
    {
      c = m_buff[m_pos++];
      return true;
    }
    // Simulate delayed eof behaviour.
    ++m_pos;
    return false;
  }

  void write(char c) override
  {
    assert(m_pos < m_cap);
    m_buff[m_pos++] = c;
    m_size = std::max(m_size, m_pos);
  }

  bool eof() const override
  {
    assert(m_pos <= m_cap + 1);
    assert(m_pos <= m_size + 1);
    return m_pos == m_size + 1;
  }

  void rewind() override { m_pos = 0; }
  void truncate() override { m_size = 0; }

private:
  size_t m_pos = 0;
  size_t m_size = 0;
  size_t m_cap = 0;
  Buff & m_buff;
};
}  // namespace io
