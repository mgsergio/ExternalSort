#include "buffered_string_wrapper.hh"

#include <algorithm>
#include <cassert>

namespace io
{
bool BufferedStringWrapper::read(StringView & s)
{
  assert(m_lastOp != LastOp::Write);
  m_lastOp = LastOp::Read;

  if (eof() && m_pos == m_end)
    return false;

  auto strEnd = m_pos;
  while (strEnd < m_end && m_buff[strEnd] != '\n')
    ++strEnd;

  if (strEnd != m_end)
  {
    s = StringView(&m_buff.data()[m_pos], strEnd - m_pos);
    m_pos = strEnd + 1;
    return true;
  }

  fetch();
  if (m_end == 0)
    return false;

  strEnd = m_pos;
  while (strEnd < m_end && m_buff[strEnd] != '\n')
    ++strEnd;

  // Either buffer is to small or there is no newline at the end.
  if (strEnd == m_end)
    return false;

  s = StringView(&m_buff.data()[m_pos], strEnd - m_pos);
  m_pos = strEnd + 1;

  return true;
}

void BufferedStringWrapper::write(StringView const & s)
{
  assert(m_lastOp != LastOp::Read);
  m_lastOp = LastOp::Write;

  for (size_t written = 0; written != s.size();)
  {
    auto spaceLeft = empty() ? m_buff.size() : m_buff.size() - m_pos;
    if (spaceLeft == 0)
    {
      flush();
      spaceLeft = m_buff.size();
    }
    for (size_t i = 0; i < std::min(spaceLeft, s.size() - written); ++i)
    {
      assert(m_pos < m_buff.size());
      assert(written < s.size());
      m_buff[m_pos++] = s[written++];
    }
  }
  if (m_pos == m_buff.size())
    flush();
  // TODO(mgsergio): Should those lines go befor if (m_pos == m_buff.size())
  // since we'd like to force the buffer to be able to hold the whole string.
  m_buff[m_pos++] = '\n';
  m_end = m_pos;
}

void BufferedStringWrapper::flush()
{
  if (m_lastOp != LastOp::Write)
    return;
  m_stream.write(m_buff.data(), m_pos);
  m_pos = 0;
  m_end = 0;
}

bool BufferedStringWrapper::eof() const
{
  assert(m_lastOp != LastOp::Write);
  return m_stream.eof() && m_pos == m_end;
}

void BufferedStringWrapper::rewind()
{
  flush();
  m_stream.rewind();
  init();
}

void BufferedStringWrapper::truncate()
{
  m_stream.truncate();
  init();
}

void BufferedStringWrapper::init()
{
  m_pos = 0;
  m_end = 0;
  m_lastOp = LastOp::Unknown;
}

void BufferedStringWrapper::fetch()
{
  if (m_pos != 0)
  {
    auto const it = move(begin(m_buff) + m_pos, end(m_buff), begin(m_buff));
    m_pos = distance(begin(m_buff), it);
  }
  auto const bytesRead = m_stream.read(&m_buff.data()[m_pos], m_buff.size() - m_pos);
  m_end = m_pos + bytesRead;
  m_pos = 0;
}

bool BufferedStringWrapper::empty() const
{
  return m_pos == 0 && m_end == 0;
}
}  // namespace io
