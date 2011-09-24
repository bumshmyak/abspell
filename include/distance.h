#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

using std::vector;
using std::string;
using std::min;
using std::max;

using std::cerr;
using std::cout;
using std::cin;
using std::endl;

struct TTraversalPosition {
  TTraversalPosition() 
    : position_first_(0)
    , position_second_(0)
    , distance_(0)
  {}

  int position_first_;
  int position_second_;
  int distance_;
};

inline int delta_to_index(int delta) {
  return abs(delta) * 2 + (delta > 0 ? 1 : 0);
}

template <typename T>
T min(const T& a, const T& b, const T& c) {
  return min(a, min(b,c));
}

int get_levenshtein_distance(const string& first_line, const string& second_line, int max_distance) {
  int max_value = first_line.size() + second_line.size();
  if (max_distance <= 0) {
    max_distance = max_value;
  }
  vector<int> front(first_line.size(), max_value);
  vector<int> new_front(first_line.size(), max_value);
  for (int i = 0; i < front.size(); ++i) {
    front[i] = i + 1;
  }

  for (int second_index = 0; second_index < second_line.size(); ++second_index) {
    for (int first_index = max(0, second_index - max_distance); first_index < first_line.size() && first_index < second_index + max_distance + 1; ++first_index) {
      if (first_index == 0) {
        new_front[first_index] = min(second_index + 1, front[first_index] + 1, second_index + (first_line[first_index] == second_line[second_index] ? 0 : 1));
      } else {
        new_front[first_index] = min(new_front[first_index - 1] + 1, front[first_index] + 1, front[first_index - 1] + (first_line[first_index] == second_line[second_index] ? 0 : 1));
      }
    }
    front.swap(new_front);
  }
  return front.back();
}

int get_levenshtein_distance(const string& first_line, const string& second_line) {
  get_levenshtein_distance(first_line, second_line, 0);
}

template <typename TOutIterator>
void make_ngrams(const string& text, TOutIterator out, int ngramm_length = 2) {
  if (text.size() + 1 < ngramm_length) {
    return;
  }
  *out = "_" + text.substr(0, ngramm_length - 1);
  ++out;
  for (size_t index = 0; index + ngramm_length <= text.size(); ++index) {
    *out = text.substr(index, ngramm_length);
    ++out;
  }
  *out = text.substr(text.size() + 1 - ngramm_length , ngramm_length - 1) + "_";
  ++out;
}

double get_ngramm_distance(const string& first_line, const string& second_line, int ngramm_length = 2) {
  vector<string> first_ngramm_list;
  make_ngrams(first_line, back_inserter(first_ngramm_list), ngramm_length);
  sort(first_ngramm_list.begin(), first_ngramm_list.end());

  vector<string> second_ngramm_list;
  make_ngrams(second_line, back_inserter(second_ngramm_list), ngramm_length);
  sort(second_ngramm_list.begin(), second_ngramm_list.end());

  if (first_ngramm_list.size() == 0 && second_ngramm_list.size() == 0) {
    return 0.0;
  }
  if (first_ngramm_list.size() == 0 xor second_ngramm_list.size() == 0) {
    return 1.0;
  }

  const string* last_passed_ngramm_ptr = NULL;
  int match_count = 0;
  for (size_t first_index = 0, second_index = 0; first_index < first_ngramm_list.size() && second_index < second_ngramm_list.size(); ) {
    if (first_ngramm_list[first_index] < second_ngramm_list[second_index]) {
      ++first_index;
    } else if (first_ngramm_list[first_index] > second_ngramm_list[second_index]) {
      ++second_index;
    } else {
      if (last_passed_ngramm_ptr == NULL ||
          *last_passed_ngramm_ptr != first_ngramm_list[first_index]) {
        ++match_count; 
      }
      last_passed_ngramm_ptr = &first_ngramm_list[first_index];
      ++first_index;
      ++second_index;
    }
  }
  return 1.0 - (2.0 * match_count) / (first_ngramm_list.size() + second_ngramm_list.size());
}

