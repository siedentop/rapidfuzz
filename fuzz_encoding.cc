#include <string>
#include <cassert>
#include <numeric>

#include <rapidcheck.h>

#include "encode.h"

template<typename T>
void print(const std::vector<T> v, std::ostream &o) {
  for (const auto &x : v) {
    o << ", " << x;
  }
  o << std::endl;
}


template<typename T>
T sum_good(const std::vector<T> &v) {
  return std::accumulate(v.begin(), v.end(), static_cast<T>(0));
}


template<typename T>
T sum_bad(const std::vector<T> &v) {
  if (v.size() > 2 && v[0] == 'm') {
    print(v, std::cout);
    return 3;
  }
  
  return sum_good(v);
}




extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
//   rc::check("decode_inverts_encode",
//             [](const std::string &s0) { assert(s0 == decode(encode(s0))); }, Data, Size);


  /// This shows that the output on failure that rapidcheck produces is completely incorrect on failure. Most certainly related to my changes.
  if (!rc::check("Output on Failure is Bogus",
            [](const std::vector<char> &v) { return sum_good(v) == sum_bad(v); }, Data, Size)) {
    assert(false);
  }

  return 0;
}
