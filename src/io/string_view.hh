#pragma once

#include <cstddef>
#include <string>

namespace io
{
class StringView
{
public:
  StringView(char const * buff, size_t size)
    : m_buff(buff)
    , m_size(size)
  {
  }

  StringView(std::string const & s)
    : StringView(s.data(), s.size())
  {
  }

  StringView() = default;

  char const & operator[](size_t i) const;

  size_t size() const { return m_size; }
  bool empty() const { return m_size == 0; }

private:
  char const * m_buff = nullptr;
  size_t m_size = 0;
};

bool operator<(StringView const & s1, StringView const & s2);
bool operator<=(StringView const & s1, StringView const & s2);
bool operator>(StringView const & s1, StringView const & s2);
bool operator>=(StringView const & s1, StringView const & s2);
bool operator==(StringView const & s1, StringView const & s2);
bool operator!=(StringView const & s1, StringView const & s2);
}  // namespace io
