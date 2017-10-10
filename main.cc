#include <iostream>
#include <string>

#include <rapidcheck.h>

bool callable(int x) {
  std::cout << x << std::endl;
  return x == 0;
}

template <typename Testable>
void convert(Testable &&testable) {
  /* See rapidcheck/detail/Property.hpp, where I stole this from. */
  // using namespace rc::detail;
  using Adapter = rc::detail::PropertyAdapter<rc::Decay<Testable>>;

  //  rc::detail::ArgTypes<Testable> args;

  Adapter(std::forward<Testable>(testable));

  // testable(args);

  // rc::detail::applyTuple(args, testable);
}

int main() {

  convert(callable);

  uint8_t Data[33] = "randomIsCoolLeetrandomIsCoolLeet";

  rc::check(
      "string call", [](const std::string &s) { return s == "x"; }, Data, 32);

  // TODO: why doen't these work? They never calls Random::next()!
  // Detects as 'falsifiable'
  rc::check("", [](int x) { return x != 0; }, Data, 32);
  // Detects as 'falsifiable'
  rc::check("", [](int x) { return x == 1; }, Data, 32);

  // Detects as 'falsifiable' only with max_success > 1.
  // Therefore, I assume that it always tries some default values.
  rc::check("call-me", callable, Data, 32);

  // This on the other hand is called.
  // Maybe, the compiler can eliminate this at compile time? That would be
  // crazy.
  // No, modifying it above yields simply the wrong result.
  rc::check(
      "vector", [](std::vector<int> x) { return x.size() == 2; }, Data, 32);

  std::cout << "Hello, World!" << std::endl;

  return 0;
}
