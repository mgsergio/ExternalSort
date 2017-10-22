#include "file_stream.hh"

#include <cassert>
#include <cerrno>

namespace io
{
FileStream::FileStream(FILE * file, std::string const & mode)
  : m_file(file)
  , m_mode(mode)
{
  assert(m_file != nullptr);
}

FileStream::FileStream(std::string const & fileName, std::string const & mode)
  : m_mode(mode)
  , m_own(true)
{
  m_file = fopen(fileName.data(), m_mode.data());
  if (!m_file)
    perror(strerror(errno));
  assert(m_file);
}

FileStream::~FileStream()
{
  if (m_own)
  {
    assert(m_file != nullptr);
    fclose(m_file);
  }
}

bool FileStream::eof() const
{
  return feof(m_file);
}

void FileStream::rewind()
{
  ::rewind(m_file);
}

void FileStream::truncate()
{
  m_file = freopen(nullptr /* filename */, m_mode.data(), m_file);
}

bool FileStream::read(char & c)
{
  int ch = 0;
  ch = fgetc(m_file);
  if (ch == EOF)
    return false;
  c = static_cast<char>(ch);
  return true;
}

size_t FileStream::read(char buff[], size_t size)
{
  return static_cast<size_t>(fread(buff, sizeof(char), size, m_file));
}

void FileStream::write(char c)
{
  fputc(c, m_file);
}

size_t FileStream::write(char const buff[], size_t size)
{
  return fwrite(buff, sizeof(char), size, m_file);
}
}  // namespace io
