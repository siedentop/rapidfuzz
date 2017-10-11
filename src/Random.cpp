#include "rapidcheck/Random.h"

#include <cassert>
#include <functional>

#include "rapidcheck/Show.h"

// A lot of this code is taken from https://github.com/wernerd/Skein3Fish but
// highly modified.

namespace rc {

// Random::Random()
//     : Random(Key{{0, 0, 0, 0}}) {}

Random::Random()
    : m_data(nullptr) {}

// Random::Random(const Key &key)
//     : m_key(key)
//     , m_block()
//     , m_bits(0)
//     , m_counter(0)
//     , m_bitsi(0) {}

// We just repeat the seed in the key
// Random::Random(uint64_t seed)
//    : Random(Key{{seed, seed, seed, seed}}) {}

Random::Random(const FuzzData::Ptr &data)
    : m_data(data) {}

Random Random::split() {
  /// Split is not needed anymore, it is simply a copy of the underlying shared-ptr to the data-stack.
  return Random(m_data);
}

Random::Number Random::next() {
  if (m_data->empty()) {
    throw RandomEmptyException();
  }

  const auto value = m_data->top_and_pop();

  return value;
}

bool operator==(const Random &lhs, const Random &rhs) {
  // TODO: who uses this and why? Also, implement it.
  assert(false);
  return true;
}

bool operator!=(const Random &lhs, const Random &rhs) { return !(lhs == rhs); }

bool operator<(const Random &lhs, const Random &rhs) {
  // This is lexicographic comparison!
  //  return std::tie(             lhs.m_key, lhs.m_block, lhs.m_bits,
  //  lhs.m_counter, lhs.m_bitsi) <
  //      std::tie(rhs.m_key, rhs.m_block, rhs.m_bits, rhs.m_counter,
  //      rhs.m_bitsi);

  // TODO: Need to implement
  assert(false);
  return true;
}

std::ostream &operator<<(std::ostream &os, const Random &random) {
  // TODO: implement!
  assert(false);
  return os;
}

} // namespace rc
