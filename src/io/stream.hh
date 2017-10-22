#pragma once

#include <cstddef>

namespace io
{
class IStream
{
public:
  virtual ~IStream() = default;

  virtual bool eof() const = 0;

  virtual void rewind() = 0;
  virtual void truncate() = 0;

  virtual bool read(char & c) = 0;
  virtual size_t read(char buff[], size_t size) = 0;

  virtual void write(char c) = 0;
  virtual size_t write(char const buff[], size_t size) = 0;
};
}  // namespace io
