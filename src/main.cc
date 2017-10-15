#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

class Stream
{
public:
  using value_type = std::string;

  Stream(std::vector<char> & buff)
    : m_size(buff.size())
    , m_buff(buff)
  {
  }

  Stream(std::vector<char> & buff, size_t size)
    : m_size(size)
    , m_buff(buff)
  {
  }

  void rewind() { m_pos = 0; }
  void truncate() { m_size = 0; }
  bool eof() const {
    assert(m_pos <= m_size);
    return m_pos == m_size;
  }

  bool read(std::string & s)
  {
    if (eof())
      return false;
    auto const start = m_pos;
    while (!eof() && m_buff[m_pos] != '\n')
      ++m_pos;
    s.resize(m_pos - start);
    for (auto i = start; i < m_pos; ++i)
    {
      assert(i - start < s.size());
      s[i - start] = m_buff[i];
    }
    if (!eof())
      ++m_pos;  // skip \n.
    return true;
  }

  void write(std::string const & s)
  {
    assert(m_pos <= m_buff.size());
    assert(s.size() <= m_buff.size() - m_pos);
    for (auto const ch : s)
      m_buff[m_pos++] = ch;
    if (m_pos < m_buff.size())
      m_buff[m_pos++] = '\n';
    m_size = std::max(m_size, m_pos);
  }

private:
  size_t m_pos = 0;
  size_t m_size = 0;
  std::vector<char> & m_buff;
};

bool split(Stream & input, Stream & left, Stream & right, size_t const by)
{
  Stream::value_type v;
  Stream * output[] = {&left, &right};
  size_t select = 0;
  bool splitted = false;
  for (size_t runLen = 0; input.read(v); ++runLen)
  {
    if (runLen == by)
    {
      runLen = 0;
      select = (select + 1) % 2;
      splitted = true;
    }
    output[select]->write(v);
  }
  return splitted;
}

void copy(Stream & src, Stream & dst, size_t n)
{
  Stream::value_type v;
  while (n-- && src.read(v))
    dst.write(v);
}

void copy(Stream & src, Stream & dst)
{
  Stream::value_type v;
  while(src.read(v))
    dst.write(v);
}

void merge(Stream & src1, Stream & src2, Stream & dst, size_t const by)
{
  Stream::value_type v1;
  Stream::value_type v2;
  while (!src1.eof() && !src2.eof())
  {
    size_t readFrom1 = 0;
    size_t readFrom2 = 0;
    bool dirty1 = true;
    bool dirty2 = true;
    while (readFrom1 < by || readFrom2 < by)
    {
      if (readFrom1 < by && dirty1)
      {
        if (!src1.read(v1)) break;
        ++readFrom1;
        dirty1 = false;
      }
      if (readFrom2 < by && dirty2)
      {
        if (!src2.read(v2)) break;
        ++readFrom2;
        dirty2 = false;
      }

      if (dirty1 || dirty2)
        break;

      if (v1 <= v2)
      {
        dst.write(v1);
        dirty1 = true;
      }
      else
      {
        dst.write(v2);
        dirty2 = true;
      }
    }

    if (!dirty1)
      dst.write(v1);
    if (by - readFrom1)
      copy(src1, dst, by - readFrom1);

    if (!dirty2)
      dst.write(v2);
    if (by - readFrom2)
      copy(src2, dst, by - readFrom2);
  }

  copy(src1, dst);
  copy(src2, dst);
}

void mergeSort(Stream & input, Stream & output, Stream & tmp)
{
  size_t currentRunLen = 1;
  while (true)
  {
    if (!split(input, output, tmp, currentRunLen))
      break;

    input.rewind();
    output.rewind();
    tmp.rewind();

    merge(output, tmp, input, currentRunLen);
    currentRunLen <<= 1;

    input.rewind();
    output.rewind();
    output.truncate();
    tmp.rewind();
    tmp.truncate();
  }
  output.rewind();
}

int main(int argc, char * argv[])
{
  char const input[] = {
    R"(CvcAfERzdhUWg
bg2iXCtPJS0bJA
UJQ6CHBLmV
PDrkVO6aNBHqmf
BSgOOl5e9em
xdA7CnK2QJKvE5
kaQqYeIlgZ)"
  };

  std::vector<char> inBuff(std::begin(input), std::end(input) - 1);
  std::vector<char> outBuff(inBuff.size());
  std::vector<char> tmpBuff(inBuff.size());

  Stream in(inBuff);
  Stream out(outBuff, 0);
  Stream tmp(tmpBuff, 0);

  mergeSort(in, out, tmp);

  out.rewind();
  Stream::value_type v;
  while (out.read(v))
    std::cout << v << std::endl;

  return 0;
}
