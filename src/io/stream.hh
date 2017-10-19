#pragma once

namespace io
{
class IStream
{
public:
  virtual bool eof() const = 0;

  virtual void rewind() = 0;
  virtual void truncate() = 0;

  virtual bool read(char & c) = 0;
  virtual void write(char c) = 0;
};
}  // namespace io
