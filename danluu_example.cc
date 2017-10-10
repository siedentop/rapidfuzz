
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "rapidcheck.h"

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

/// Takes an array and returns a non-zero int
int dut(const std::vector<int> &a) {
  if (a.size() != 4) {
    return 1;
  }

  if (some_filter(a[0]))
    if (some_filter(a[1]))
      if (some_filter(a[2]))
        if (some_filter(a[3])) {
          return 0;
        }
  return 5;
}

int fud(int x) { return x > 0; }


extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  rc::check("Fuzz-fud", fud, Data, Size);

  return 0;
}
