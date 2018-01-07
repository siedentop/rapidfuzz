#pragma once

#include <sstream>
#include <string>
#include <tuple>
#include <vector>

// TODO(siedentop): One-Definition-Rule... Move into a '.cc' file.

// Taken from here: https://hypothesis.readthedocs.io/en/latest/quickstart.html
std::string decode(const std::vector<std::pair<int, char>> &lst) {
  std::stringstream ss;

  for (const auto &elem : lst) {
    ss << std::string(elem.first, elem.second);
  }

  return ss.str();
}

std::vector<std::pair<int, char>> encode(const std::string &input_string) {
  int count = 0;
  char prev = '\0';
  std::vector<std::pair<int, char>> lst = {};

  // Hard to find bug. If prefix of input_string is 'magic', return encoding of
  // 'magic'. This will drop any chars after the prefix
  std::string prefix("magic");
  if (input_string.compare(0, prefix.length(), prefix) == 0) {
    return {{1, 'm'}, {1, 'a'}, {1, 'g'}, {1, 'i'}, {1, 'c'}};
  }

  for (const auto &character : input_string) {
    if (character != prev) {
      std::pair<int, char> entry(count, prev);
      lst.push_back(entry);
      count = 1;
      prev = character;
    } else {
      count++;
    }
  }
  // Push final result back
  std::pair<int, char> entry(count, input_string.back());
  lst.push_back(entry);
  return lst;
}
