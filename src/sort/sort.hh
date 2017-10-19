#pragma once

#include <cstddef>

namespace sort
{
namespace impl
{
template <typename Stream>
bool split(Stream & input, Stream & left, Stream & right, size_t const by)
{
  typename Stream::value_type v;
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

template <typename Stream>
void copy(Stream & src, Stream & dst, size_t n)
{
  typename Stream::value_type v;
  while (n-- && src.read(v))
    dst.write(v);
}

template <typename Stream>
void copy(Stream & src, Stream & dst)
{
  typename Stream::value_type v;
  while(src.read(v))
    dst.write(v);
}

template <typename Stream>
void merge(Stream & src1, Stream & src2, Stream & dst, size_t const by)
{
  typename Stream::value_type v1;
  typename Stream::value_type v2;
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
}  // namespace impl

template <typename Stream>
void mergeSort(Stream & input, Stream & output, Stream & tmp)
{
  size_t currentRunLen = 1;
  while (true)
  {
    if (!impl::split(input, output, tmp, currentRunLen))
      break;

    input.rewind();
    output.rewind();
    tmp.rewind();

    impl::merge(output, tmp, input, currentRunLen);
    currentRunLen <<= 1;

    input.rewind();
    output.rewind();
    output.truncate();
    tmp.rewind();
    tmp.truncate();
  }
  output.rewind();
}
}  // namespace sort
