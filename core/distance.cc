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

template <typename T>
T get_levenshtein_distance_impl(
    const string& first_line,
    const string& second_line,
    int max_distance,
    const T* cost_matrix) {

  const T& DOUBLING_COST = cost_matrix[0];
  const T& INSERT_COST = cost_matrix[1];
  const T& MISS_COST = cost_matrix[2];
  const T& REPLACE_COST = cost_matrix[3];
  const T& TRANSPOSE_COST = cost_matrix[4];
  const T& EDGE_FACTOR = cost_matrix[5];

  int max_value = first_line.size() + second_line.size();
  if (max_distance <= 0) {
    max_distance = max_value;
  }
  vector<T> front(first_line.size(), max_value);
  vector<T> new_front(first_line.size(), max_value);
  for (int i = 0; i < front.size(); ++i) {
    front[i] = (i + 1) * MISS_COST;
  }

  for (int second_index = 0; second_index < second_line.size(); ++second_index) {
    for (
        int first_index = max(0, second_index - max_distance);
        first_index < first_line.size() &&
          first_index < second_index + max_distance + 1;
        ++first_index) {
      T cost_factor = 1 + EDGE_FACTOR * exp(-1.5 * min(
          min(first_index, second_index),
          min<int>(first_line.size() - 1 - first_index,
            second_line.size() - 1 - second_index)));

      if (first_index == 0) {
        T miss_distance = (second_index + 1) * MISS_COST * cost_factor;

        T insert_distance = front[first_index] +
          ((second_index > 0 &&
            second_line[second_index] == second_line[second_index - 1]) ?
              DOUBLING_COST :
              INSERT_COST) * cost_factor;

        T replace_distance = second_index * MISS_COST * cost_factor +
          (first_line[first_index] == second_line[second_index] ?
            0 :
            REPLACE_COST * cost_factor);

        new_front[first_index] = min(miss_distance, insert_distance, replace_distance);
      } else {
        T miss_distance = new_front[first_index - 1] +
          ((first_line[first_index] == first_line[first_index - 1]) ?
            DOUBLING_COST :
            MISS_COST) * cost_factor;

        T insert_distance = front[first_index] +
          ((second_index > 0 &&
            second_line[second_index] == second_line[second_index - 1]) ?
              DOUBLING_COST :
              INSERT_COST) * cost_factor;

        T replace_distance = front[first_index - 1] +
              (first_line[first_index] == second_line[second_index] ?
                0 :
                ((second_index > 0 &&
                  first_line[first_index - 1] == second_line[second_index] &&
                  first_line[first_index] == second_line[second_index - 1]) ?
                    TRANSPOSE_COST - REPLACE_COST :
                    REPLACE_COST) * cost_factor);

        new_front[first_index] = min(miss_distance, insert_distance, replace_distance);
      }
    }
    front.swap(new_front);
  }
  return front.back();
}

double get_weighted_levenshtein_distance(
    const string& first_line,
    const string& second_line,
    int max_distance,
    const double* cost_matrix) {
  const double default_cost_matrix[] = {1, 1, 1, 1, 1, 0};
  if (cost_matrix == NULL) {
    cost_matrix = default_cost_matrix;
  }
  return get_levenshtein_distance_impl<double>(first_line, second_line, max_distance, cost_matrix);
}

int get_levenshtein_distance(
    const string& first_line,
    const string& second_line,
    int max_distance) {
  const int cost_matrix[] = {1, 1, 1, 1, 1, 0};
  return get_levenshtein_distance_impl<int>(first_line, second_line, max_distance, cost_matrix);
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
