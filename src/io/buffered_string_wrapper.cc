#include "buffered_string_wrapper.hh"

#include <algorithm>
#include <cassert>

namespace io
{
bool BufferedStringWrapper::read(StringView & s)
{
  if (eof())
    return false;

  auto strEnd = m_pos;
  while (strEnd < m_end && m_buff[strEnd != '\n'])
    ++strEnd;

  if (strEnd != m_end)
  {
    s = StringView(&m_buff.data()[m_pos], strEnd - m_pos + 1);
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
  m_buff[m_pos++] = '\n';
}

void BufferedStringWrapper::flush()
{
  m_stream.write(m_buff.data(), m_pos);
  m_pos = 0;
  m_end = 0;
}

void BufferedStringWrapper::rewind()
{
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
}

void BufferedStringWrapper::fetch()
{
  if (m_pos != 0)
  {
    auto const it = move(begin(m_buff) + m_pos, end(m_buff), begin(m_buff));
    m_pos = distance(begin(m_buff), it);
  }
  // printf("Reading from stream to buff at pos %zd, %zd bytes\n", m_pos, m_buff.size() - m_pos);
  auto const bytesRead = m_stream.read(&m_buff.data()[m_pos], m_buff.size() - m_pos);
  // printf("%zd bytes got form m_stream\n", bytesRead);
  // printf("buffer after fetch:\n");
  m_end = m_pos + bytesRead;
  m_pos = 0;
  // for (size_t i = 0; i < m_end; ++i)
  //   putc(m_buff[i], stdout);
  // putc('\n', stdout);
}

bool BufferedStringWrapper::empty() const
{
  return m_pos == 0 && m_end == 0;
}
}  // namespace io
