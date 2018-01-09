#include <cassert>
#include <string>

#include "encode.h"
#include "wrapper.h"

/** Returns the input except on the special case, where we hit a bug.
 */
std::string buggy_noop(const std::string &s) {
  const std::string key("magic");
  if (s.compare(0, key.length(), key) == 0) {
    return "magix"; // Cut-off the remainder of the input
  }

  return s;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  // rapidfuzz::check(Data, Size, [](std::string s0) { assert(s0 ==
  // buggy_noop(s0));});
  rapidfuzz::check(Data, Size, [](std::string s) {
    if (s == "magix") {
      std::cout << s << std::endl;
      assert(false);
    }
  });
  return 0;
}
