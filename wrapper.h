#pragma once // TODO: use header guards.
#include <cassert>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <type_traits>
#include <vector>
#include <utility>

#include "external/callable/callable.hpp"

namespace rapidfuzz {

class RawQueue {
public:
  RawQueue(const uint8_t *Data, size_t Size)
      : data_(Data)
      , size_(Size / sizeof(uint8_t))
      , index_(0){};

  /** Takes one element of type T from queue.
   *
   * Throws if empty.
   *
   * NOTE: Little endianess means that uint8_t {1, 0, 0, 0} == int {1}.
   */
  template <typename T>
  T pop() {
    assert(data_);
    std::lock_guard<std::mutex> lock(data_mutex_);

    static_assert(sizeof(uint8_t), "Index is wrong");

    constexpr size_t sizeof_T = sizeof(T) / sizeof(uint8_t);
    check_size(sizeof_T);

    // std::cout << "S: " << size_ << ", I: " << index_ << std::endl;

    // T val;
    // std::memcpy(reinterpret_cast<T*>(&val), &(data_[index_]), sizeof(T));
    // std::memcpy(                       &val , &(data_[index_]), sizeof(T));

    const T val = *reinterpret_cast<const T *>(&(data_[index_]));
    index_ += sizeof_T;

    return val;
  }

protected:
  bool has_size(size_t requested) const { return index_ + requested <= size_; }

  void check_size(size_t requested) const { // TODO: C++ to say this will throw?
    if (!has_size(requested)) {
      throw std::runtime_error("Queue depleted"); // TODO: Custom exception!
    }
  }

private:
  const uint8_t *data_; ///< Warning: Ownership resides outside of RawQueue.
  std::mutex data_mutex_;
  const size_t size_;
  size_t index_;
};

template <>
std::string RawQueue::pop<std::string>() {
  assert(data_);
  // check_size(1);
  // size_t string_length = static_cast<size_t>(data_[index_]); // Up-to 255
  // ought to be enough.

  size_t string_length = pop<size_t>();

  check_size(string_length);
  std::lock_guard<std::mutex> lock(data_mutex_);
  const std::string val(reinterpret_cast<const char *>(&(data_[index_])),
                        string_length);
  index_ += string_length;
  return val;
};

// TODO: read on foldables as done by RapidCheck.
// TODO: same as std::string implementation
template <>
std::vector<int> RawQueue::pop<std::vector<int>>() {
  size_t vec_length;
  {
    std::lock_guard<std::mutex> lock(data_mutex_);

    check_size(1);
    constexpr int size_method = 0;
    if constexpr (size_method == 0) {
      const size_t capacity = (size_ - index_) / sizeof(int);
      const float requested = static_cast<size_t>(data_[index_]) / 256;
      vec_length = capacity * requested;
    } else {
      vec_length = static_cast<size_t>(data_[index_]);
    }
    index_++;
    check_size(vec_length);
  } // end lock_guard

  std::vector<int> val;
  val.reserve(vec_length);
  for (int i = 0; i < vec_length; i++) {
    val.push_back(pop<int>());
  }

  return val;
}

template <typename F, typename... Args>
decltype(auto) call(RawQueue *Data, F &&f, Args &&... args) {
  if constexpr (std::is_invocable<F, Args...>::value) {
    return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
  } else {
    assert(Data);
    constexpr size_t n_already = sizeof...(args);
    constexpr size_t n_needed = callable_traits<F>::argc;
    static_assert(n_needed >= n_already, "Too many arguments!");
    constexpr size_t pos = n_already;
    typedef typename callable_traits<F>::template argument_type<pos> T;
    auto val = Data->pop<T>();
    return call(Data, std::forward<F>(f), std::forward<Args>(args)..., val);
  }
}

} // namespace rapidfuzz
