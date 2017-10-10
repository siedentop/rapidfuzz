#pragma once

#include <memory>
#include <stack>

namespace rc {

/// Wrapper around a std::stack. + nice c-tor.
class RandomData {
public:
  using Ptr = std::shared_ptr<RandomData>;

  RandomData(uint8_t *Data, size_t Size);

  inline bool empty() const { return m_data.empty(); }

  /// Use top value from stack (so it's gone afterwards).
  inline uint64_t top_and_pop() {
    const auto value = m_data.top();
    m_data.pop();
    return value;
  }

private:
  std::stack<uint64_t> m_data;
};

} // namespace rc
