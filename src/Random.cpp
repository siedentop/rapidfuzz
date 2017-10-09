#include "rapidcheck/Random.h"

#include <cassert>
#include <functional>
#include <iostream>

#include "rapidcheck/Show.h"

// A lot of this code is taken from https://github.com/wernerd/Skein3Fish but
// highly modified.

namespace rc {

// Random::Random()
//     : Random(Key{{0, 0, 0, 0}}) {}

Random::Random()
    : m_data({})
    , m_index(42) // This way, it is emtpy...
{}

// Random::Random(const Key &key)
//     : m_key(key)
//     , m_block()
//     , m_bits(0)
//     , m_counter(0)
//     , m_bitsi(0) {}

// We just repeat the seed in the key
// Random::Random(uint64_t seed)
//    : Random(Key{{seed, seed, seed, seed}}) {}

Random::Random(const RandomData &data, size_t index)
    : m_data(data)
    , m_index(index) {}

Random Random::split() {
  std::cout << "split called @ " << m_index << std::endl;
  Random right(m_data, 2 * m_index + 1);
  return right;
}

Random::Number Random::next() {
  if (empty()) {
    throw RandomEmptyException(m_data.size(), m_index);
  }

  std::cout << "next called @ " << m_index << std::endl;

  const auto value = m_data[m_index];

  m_index = 2 * m_index;
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
