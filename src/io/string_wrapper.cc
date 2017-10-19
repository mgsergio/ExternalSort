#include "string_wrapper.hh"

namespace io
{
bool StringWrapper::read(std::string & s)
{
  std::string tmp;
  char c = '\0';
  if (!m_stream.read(c))
    return false;
  while (!eof() && c != '\n')
  {
    tmp += c;
    m_stream.read(c);
  }

  // Enforce all strings to and with \n.
  if (eof())
    return false;

  s.swap(tmp);

  return true;
}

void StringWrapper::write(std::string const & s)
{
  for (auto const c : s)
    m_stream.write(c);
  m_stream.write('\n');
}
}  // namespace io
