#include "string_view.hh"

#include <cassert>

namespace
{
int compare(io::StringView const & s1, io::StringView const & s2)
{
  size_t i = 0;
  size_t j = 0;
  while (i < s1.size() && j < s2.size())
  {
    if (s1[i] != s2[j])
      return s1[i] - s2[j];
    ++i;
    ++j;
  }
  return s1.size() - s2.size();
}
}  // namespace

namespace io
{
char const & StringView::operator[](size_t i) const
{
  assert(i < m_size);
  return m_buff[i];
}

bool operator<(StringView const & s1, StringView const & s2) { return compare(s1, s2) < 0; }
bool operator<=(StringView const & s1, StringView const & s2) { return compare(s1, s2) <= 0; }
bool operator>(StringView const & s1, StringView const & s2) { return compare(s1, s2) > 0; }
bool operator>=(StringView const & s1, StringView const & s2) { return compare(s1, s2) >= 0; }
bool operator==(StringView const & s1, StringView const & s2) { return compare(s1, s2) == 0; }
bool operator!=(StringView const & s1, StringView const & s2) { return compare(s1, s2) != 0; }
}  // namespace io
