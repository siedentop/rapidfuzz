#include <rapidcheck.h>
#include <rapidcheck/state.h>

using namespace rc;

class Counter {
public:
  void inc() { m_value++; }

  void dec() {
    assert(m_value > 0);
    // Broken!
    if (m_value != 10) {
      m_value--;
    }
  }

  int get() { return m_value; }

private:
  int m_value = 0;
};

struct CounterModel {
  int value = 0;
};

struct Inc : public state::Command<CounterModel, Counter> {
  void apply(CounterModel &s0) const override { s0.value++; }

  void run(const CounterModel &s0, Counter &counter) const override {
    counter.inc();
    assert(counter.get() == (s0.value + 1));
  }
};

struct Dec : public state::Command<CounterModel, Counter> {
  void checkPreconditions(const CounterModel &s0) const override {
    RC_PRE(s0.value > 0);
    assert(s0.value > 0);
  }

  void apply(CounterModel &s0) const override { s0.value--; }

  void run(const CounterModel &state, Counter &counter) const override {
    counter.dec();
      if (counter.get() != (state.value - 1)) {
          std::cout << "Failing: " << counter.get() << ", " << state.value -1 << std::endl; }
    assert(counter.get() == (state.value - 1));
  }
};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
//int main() {
//    const uint8_t* Data = nullptr;
//    size_t Size = 0;

    
  check([] {
    CounterModel state;
    Counter sut;
    state::check(state, sut, state::gen::execOneOfWithArgs<Inc, Dec>());
  }, Data, Size);

  return 0;
}
