#include <iostream>

#include "rapidcheck/detail/FuzzData.h"

namespace rc {
FuzzData::FuzzData(const uint8_t *Data, size_t Size) {
  // TODO: really stupid way to fill the container
  const size_t size64 = Size / 8;
  auto Data64 = reinterpret_cast<const uint64_t *>(Data);
  for (size_t i = 0; i < size64; i++) {
    m_data.push(Data64[i]);
  }
}
} // namespace rc
