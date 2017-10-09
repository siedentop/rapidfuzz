#pragma once

#include <cstdint>
#include <iosfwd>

namespace rc {
namespace detail {

struct Data {
  /// Pointer to array
  uint8_t *ptr;
  /// Size of array
  size_t size;
};

/// Describes the parameters for a test.
struct TestParams {
  /// The seed to use.
  uint64_t seed = 0;
  /// The maximum number of successes before deciding a property passes.
  int maxSuccess = 100;
  /// The maximum size to generate.
  int maxSize = 100;
  /// The maximum allowed number of discarded tests per successful test.
  int maxDiscardRatio = 10;
  /// Whether shrinking should be disabled or not.
  bool disableShrinking = false;

  /// Pre-filled data
  Data data;
};

bool operator==(const TestParams &p1, const TestParams &p2);
bool operator!=(const TestParams &p1, const TestParams &p2);

std::ostream &operator<<(std::ostream &os, const TestParams &params);

} // namespace detail
} // namespace rc
