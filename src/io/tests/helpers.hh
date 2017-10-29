#pragma once

#include <initializer_list>

namespace io
{
namespace tests
{
template <typename Sink, typename StringLike>
void writeStringToSink(Sink & sink, std::initializer_list<StringLike> const l)
{
  for (auto const & s : l)
    sink.write(s);
}
}  // namespace tests
}  // namespace io
