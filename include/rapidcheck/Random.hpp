#pragma once

#include "rapidcheck/detail/Serialization.h"

namespace rc {

template <typename Iterator>
Iterator serialize(const Random &random, Iterator output) {
  using namespace rc::detail;
  auto oit = output;
  return ++oit;
}

template <typename Iterator>
Iterator deserialize(Iterator begin, Iterator end, Random &output) {
  using namespace rc::detail;
  auto iit = begin;

  Random::Counter counter;
  iit = deserializeCompact(iit, end, counter);
  // Normally, the block is calculated lazily if counter is divisible by 4 so
  // let's simulate this.
  if (counter != 0) {
    const auto blki =
        ((counter - 1) % std::tuple_size<Random::Block>::value) + 1;
    if (blki != 0) {
      // Calculate the block as if counter % 4 == 0
    }
  }
  return ++iit;
}

} // namespace rc
