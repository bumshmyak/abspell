#include <using_std.h>
#include <cmath>
#include <cassert>
#include <utility>
#include <cstring>

using std::pair;

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

int get_levenshtein_distance(
    const string& first_line,
    const string& second_line,
    int max_distance) {

  static const int INSERT_COST = 1;
  static const int REPLACE_COST = 1;
  static const int TRANSPOSE_COST = 1; // must be >= REPLACE_COST
  static const int MISS_COST = 1;


  int max_value = first_line.size() + second_line.size();
  if (max_distance <= 0) {
    max_distance = max_value;
  }
  vector<int> front(first_line.size(), max_value);
  vector<int> new_front(first_line.size(), max_value);
  for (int i = 0; i < front.size(); ++i) {
    front[i] = (i + 1) * MISS_COST;
  }

  for (int second_index = 0; second_index < second_line.size(); ++second_index) {
    for (
        int first_index = max(0, second_index - max_distance);
        first_index < first_line.size() &&
          first_index < second_index + max_distance + 1;
        ++first_index) {
      if (first_index == 0) {
        new_front[first_index] = min(
            (second_index + 1) * MISS_COST,
            front[first_index] + INSERT_COST,
            second_index * MISS_COST +
              (first_line[first_index] == second_line[second_index] ? 0 : REPLACE_COST));
      } else {
        new_front[first_index] = min(
            new_front[first_index - 1] + MISS_COST,
            front[first_index] + INSERT_COST,
            front[first_index - 1] +
              (first_line[first_index] == second_line[second_index] ?
                0 :
                REPLACE_COST + (
                  second_index > 0 &&
                  first_line[first_index - 1] == second_line[second_index] &&
                  first_line[first_index] == second_line[second_index - 1] ?
                    TRANSPOSE_COST - 2 * REPLACE_COST :
                    0)));
      }
    }
    front.swap(new_front);
  }
  return front.back();
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
  // 123
}

double get_ngramm_jaccard_distance(
    const string& first_line,
    const string& second_line,
    int ngramm_length) {
  vector<string> first_ngramm_list;
  make_ngrams(first_line, back_inserter(first_ngramm_list), ngramm_length);
  sort(first_ngramm_list.begin(), first_ngramm_list.end());

  vector<string> second_ngramm_list;
  make_ngrams(second_line, back_inserter(second_ngramm_list), ngramm_length);
  sort(second_ngramm_list.begin(), second_ngramm_list.end());

  if ((first_ngramm_list.size() == 0) || (second_ngramm_list.size() == 0)) {
    return 1.0;
  }

  vector<string> intersection_result(min(first_ngramm_list.size(),
                                         second_ngramm_list.size()));
  int intersection_count =
      set_intersection(first_ngramm_list.begin(),
                       first_ngramm_list.end(),
                       second_ngramm_list.begin(),
                       second_ngramm_list.end(),
                      intersection_result.begin()) -
      intersection_result.begin();
  
  vector<string> union_result(first_ngramm_list.size() +
                              second_ngramm_list.size());
  int union_count =
      set_union(first_ngramm_list.begin(),
                first_ngramm_list.end(),
                second_ngramm_list.begin(),
                second_ngramm_list.end(),
                union_result.begin()) -
      union_result.begin();

  return 1.0 - 1.0 * intersection_count / union_count;
}


double get_ngramm_dice_distance(
    const string& first_line,
    const string& second_line,
    int ngramm_length) {
  vector<string> first_ngramm_list;
  make_ngrams(first_line, back_inserter(first_ngramm_list), ngramm_length);
  sort(first_ngramm_list.begin(), first_ngramm_list.end());

  vector<string> second_ngramm_list;
  make_ngrams(second_line, back_inserter(second_ngramm_list), ngramm_length);
  sort(second_ngramm_list.begin(), second_ngramm_list.end());

  if (first_ngramm_list.size() == 0 || second_ngramm_list.size() == 0) {
    return 1.0;
  }

  vector<string> intersection_result(min(first_ngramm_list.size(),
                                         second_ngramm_list.size()));
  int intersection_count =
      set_intersection(first_ngramm_list.begin(),
                       first_ngramm_list.end(),
                       second_ngramm_list.begin(),
                       second_ngramm_list.end(),
                      intersection_result.begin()) -
      intersection_result.begin();

  return 1.0 - 2.0 * intersection_count /
      (first_ngramm_list.size() + second_ngramm_list.size());
}

const char* qwerty_map[] = {
  "`1234567890-=",
  "qwertyuiop[]\\",
  "asdfghjkl;'",
  "zxcvbnm,./" };

pair<int, int> find_char_at_qwerty(char c) {
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < strlen(qwerty_map[row]); ++col) {
      if (qwerty_map[row][col] == c) {
        return std::make_pair(row, col);
      }
    }
  }
  return std::make_pair(-1, -1);
}

int get_qwerty_keybord_distance(
    char first,
    char second) {
  pair<int, int> first_pos = find_char_at_qwerty(first);
  pair<int, int> second_pos = find_char_at_qwerty(second);
  assert(first_pos.first != -1);
  assert(second_pos.first != -1);
  return abs(first_pos.first - second_pos.first) +
      abs(first_pos.second - second_pos.second);
}
