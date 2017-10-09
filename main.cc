#include <iostream>
#include <string>

#include <rapidcheck.h>
// #include <rapidcheck/detail/Property.hpp>
#include <rapidcheck/detail/ApplyTuple.h>

bool callable(int x) {
  std::cout << x << std::endl;
  return true;
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
  // uint8_t Data[16] = "randomIs";

  rc::check("string call", [](const std::string &s) { return true; }, Data, 32);

  // if (!rc::check("call-me", callable)) {
  //   return 1;
  // }

  std::cout << "Hello, World!" << std::endl;

  return 0;
}
