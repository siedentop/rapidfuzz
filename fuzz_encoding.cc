#include <string>
#include <cassert>

#include <rapidcheck.h>

#include "encode.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  rc::check("decode_inverts_encode",
            [](const std::string &s0) { assert(s0 == decode(encode(s0))); }, Data, Size);
  return 0;
}
