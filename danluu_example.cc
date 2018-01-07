
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

// #include "rapidcheck.h"

#include "wrapper.h"

/** C++ implementation of Go version from https://danluu.com/testing
 */

/// Checks that a number has its bottom bits set
bool some_filter(int x) {
  static_assert(sizeof(int) == 4, "Sizeof(int) needs to be 4 bytes");
  for (int i = 0; i < 32; i++) {
    if ((x & 1) != 1) {
      return false;
    }
    x >>= 1;
  }
  return true;
}

template <typename T>
std::ostream &print(std::ostream &os, const T &t) {
  for (const auto &x : t) {
    std::cout << ", " << x;
  }
  return os;
}

/// Takes an array and returns a non-zero int
int dut(const std::vector<int> &a) {
  if (a.size() != 4) {
    return 1;
  }

  if (some_filter(a[0]) && some_filter(a[1]) && some_filter(a[2]) &&
      some_filter(a[3])) {
    return 0; // Intentional bug: documentation promises non-zero.
  }

  return 5;
}

int fud(int x) { return x == 0; }

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
///  rc::check("Fuzz-fud",
///            [](const std::vector<int> &x) { assert(dut(x) != 0); },
///            Data,
///            Size);


  rapidfuzz::RawQueue raw(Data, Size);
  try {
   rapidfuzz::call(&raw, [](std::vector<int> x) { assert(dut(x) != 0); });
    // rapidfuzz::call(&raw, [](int x) { return x > 0; });
    
  } catch (std::runtime_error) {
  }
  return 0;
}
