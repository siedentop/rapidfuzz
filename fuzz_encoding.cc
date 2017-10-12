#include <cassert>
#include <numeric>
#include <string>

#include <rapidcheck.h>

#include "encode.h"

template <typename T>
void print(const T v, std::ostream &o) {
  for (const auto &x : v) {
    o << ", " << x;
  }
  o << std::endl;
}

template <typename T>
T sum_good(const std::vector<T> &v) {
  return std::accumulate(v.begin(), v.end(), static_cast<T>(0));
}

template <typename T>
T sum_bad(const std::vector<T> &v) {
  if (v.size() > 3)
    if (v[0] == 'm')
      if (v[1] == 'a')
        if (v[2] == 'g') {
          print(v, std::cout);
          return 3;
        }

  return sum_good(v);
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  rc::check("decode_inverts_encode",
            [](const std::string &s0) { assert(s0 == decode(encode(s0))); },
            Data,
            Size);

  /// This shows that the output on failure that rapidcheck produces is
  /// completely incorrect on failure. Most certainly related to my changes.
  // rc::check(
  //     "Output on Failure is Bogus",
  //     [](const std::vector<char> &v) { assert(sum_good(v) == sum_bad(v)); },
  //     Data,
  //     Size);
  return 0;
}
