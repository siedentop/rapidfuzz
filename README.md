RapidFuzz
==========

This is an experiment of hacking around `RapidCheck` to combine RapidCheck's
property-based testing with Fuzzying. It was very much influenced by Dan Luu's
[post](https://danluu.com/testing) which suggested exactly this combination.


> RapidCheck is a C++ framework for property based testing inspired by QuickCheck
and other similar frameworks. In property based testing, you state facts about
your code that given certain precondition should always be true. RapidCheck
then generates random test data to try and find a case for which the property
doesn't hold. (From the original README)

## Aim of this experiment
When using *libFuzzer* in C++ you have to use it in the same way you would for
C: Implement a function `int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)`.

Honestly, this is cumbersome in C++. Especially, if you've seen the ease of use
present in RapidCheck. In RapidCheck you can get started by simply calling
`rc::check("some description", callable)`. `callable` can be any function object
with a wide variety of arguments. RapidCheck's magic figures out how to generate
a `std::string`, `int`, or `std::vector<float>` (and many more).

In Dan Luu's post, he urges that property-based testing (like RapidCheck) be
combined with targeted fuzzers (like LLVM's libFuzzer and Michal Zalewski's AFL). This makes sense. Currently, Fuzzying is used primarily for finding
crashes that can be induced in programs. However, most developers also care
about correct functionality.

He gives one specific example where fuzzers shine: Detecting that a very
specific input causes the wrong result. Coverage-guided fuzzying helps here,
because it tries to reach all corners of the code. QuickCheck and RapidCheck are lacking this guided approach. Instead they reason about the inputs.

Here's the example
```
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
```

I also tried it on one other but similar example. Detecting that a string encoding
function produces the wrong data if the string starts with the prefix "magic".

From an initial comparison of libFuzzer and RapidCheck, I knew that libFuzzer
detects this case in mere seconds. RapidCheck never finished the 5-letter prefix
and took a minute for the two letter case.

## Why RapidCheck? ##
This is from the original README.

> There are existing implementations of property based testing but the ones that I have found are either (in my humble opinion) a bit clunky or are missing essential features such as test case shrinking.

> Let's throw together a list of features:

> - Write your properties in an imperative way that makes sense for C++
- Test case shrinking
- Great support for STL types, including maps and sets
- Advanced combinators for creating your own generators
- Stateful based on commands in the vein of Erlang QuickCheck
- Integration with popular testing frameworks such as Boost Test, Google Test and Google Mock

I would like to retain most of the features. Especially the first feature: "that makes sense for C++".
Definitely not part of  the features should be "test case shrinking". And integration of popular testing frameworks is certainly optional.

## Results / Interpretation

* Yes, it works. But whereas the magic prefix 'magic' which leads to an error is
discovered immediately with vanilla libFuzzer, this variant is as bad as
RapidCheck in discovering it.
* Or maybe, it is even worse than RapidCheck: All the weight but smaller brain.
Finding just the prefix 'm' takes ages. As I remember RapidCheck handled that
short prefix reasonably well and only faltered with longer prefixes.
  * Trying to find `std::string("ma")` is much slower than finding
  `std::vector<char> {'m', 'a'}`. The latter can be found in a few seconds.

Next steps:
* As a next step, to analyse why this is so inefficient, I thought it would be
useful to analyse some coverage reports. My guess is that a lot of effort is
spent in the 'RapidCheck' code and little inside the relevant code.
* A re-implementation would make a lot of sense. However, this experiment showed
 me that I could not implement something like RapidCheck myself. Also, it would
 be great to keep all the features of RapidCheck.
* If re-implementation is not an option (mostly because of all the useful
  features: STL types, states, pre-conditions, generators), then knowing a better
  injection point than `Random` would be very welcome. Maybe it is the generators
  that could be used?

As always, "Next steps" do not mean I will do this. If I had the time, I would
learn how to find the required argument types to a function. And then generate
the appropriate objects from a uint8 array. But I doubt I will have the time.

### Further Notes
* The output results are wrong. This is probably a serious bug. Printing
the results manually near the error helps.
* If the unit-under-test 'asserts' then testing stops immediately (which is what we want). Using the return code of `rc::check`  takes somehow skips errors.


## Prerequisites and installation ##
[...] All the previous content has been removed because this is just a very ugly hack.

1. Build using the normal CMake procedure.
* Use `CC` set to _newest_ Clang (> 5.0), `CXX` to `clang++`, and LD to `clang++`
 too. Make sure you built Clang with compiler-rt support.
* This also works with Clang 5.0 (see tutorial.libfuzzer.info); but then you
need to change `-fsanitize=address,fuzzer` into something else.
The exact flag is well documented on that tutorial page. Also, I ended up
hardcording where the built `libFuzzer.a` is. Worked well, though.
Clang on latest is easier to use.

2. Run `./fuzz_encoding` and `./fuzz_danluu_example`. The `counter` example also works.
* Useful options: `./fuzz_encoding  CORPUS_DIR`. This will remember interesting inputs from multiple runs.
* The result is underwhelming. See detailed analysis above.

## Implementation (Hacking) Detail
* I passed `LLVMFuzzerTestOneInput`'s `Data` `uint8_t` array all the way to the first real constructor of `Random`.
* Random was completely made non-random.
    * It is implemented as a shared pointer to a stack. The pointer is shared with Random instances created through `Random::split`.
    * The stack should not be a stack, since the input data is just removed never added. A single constant container with an increasing index into it, would suffice.
    * Once `Random` is exhausted, an exception is thrown and caught. This is considered a _successful_ run.



## Thanks ##
RapidCheck was developed by Emil Eriksson at Spotify.
> Big thanks to my employer, Spotify, for making it possible for me to spend work time improving RapidCheck.

Again, thanks to RapidCheck. It is amazingly useful and very easy to use. It also
showed me new things in C++ and reminded me how much I still do not know.
