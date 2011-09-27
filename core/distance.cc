#include <using_std.h>
#include <cmath>

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
    for (
        int first_index = max(0, second_index - max_distance);
        first_index < first_line.size() &&
          first_index < second_index + max_distance + 1;
        ++first_index) {
      if (first_index == 0) {
        new_front[first_index] = min(
            second_index + 1,
            front[first_index] + 1,
            second_index +
              (first_line[first_index] == second_line[second_index] ? 0 : 1));
      } else {
        new_front[first_index] = min(
            new_front[first_index - 1] + 1,
            front[first_index] + 1,
            front[first_index - 1] +
              (first_line[first_index] == second_line[second_index] ? 0 : 1));
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
