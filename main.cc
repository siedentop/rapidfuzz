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

  uint8_t Data[32] = "randomIsCoolLeet";

  rc::check("string call", [](const std::string &s) { return true; }, Data, 32);

  rc::check("call-me", callable, Data, 32);

  std::cout << "Hello, World!" << std::endl;

  return 0;
}
