#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <map>
#include <set>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using std::map;
using std::set;

const char SEPARATOR = '\t';

struct Sample {
  string original;
  vector<string> corrections;
};

std::istream& operator>>(std::istream& in, Sample& sample) {
  sample.original.clear();
  sample.corrections.clear();
  string line;
  if (!getline(in, line)) {
    return in;
  }

  int separator_position = line.find(SEPARATOR);
  assert(separator_position != string::npos);
  sample.original = line.substr(0, separator_position);
  
  line = line.substr(separator_position + 1);
  separator_position = line.find(SEPARATOR);
  while (separator_position != string::npos) {
    sample.corrections.push_back(line.substr(0, separator_position));
    line = line.substr(separator_position + 1);
    separator_position = line.find(SEPARATOR);
  }
  if (!line.empty()) {
    sample.corrections.push_back(line);
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const Sample& sample) {
  out << sample.original;
  for (int i = 0; i < sample.corrections.size(); ++i) {
    out << SEPARATOR << sample.corrections[i];
  }
  return out;
}

int get_word_count(const string& phrase) {
  return std::count(phrase.begin(), phrase.end(), ' ') + 1;
} 

void split(const string& phrase, vector<string>& words) {
  std::istringstream iss(phrase);
  string word;
  while (iss >> word) {
    words.push_back(word);
  }
}

template <class Key, class Value>
std::ostream& operator<<(std::ostream& out, const map<Key, Value>& m) {
  for (typename map<Key, Value>::const_iterator i = m.begin();
       i != m.end();
       ++i) {
    out << i->first << ' ' << i->second << endl;
  }
  return out;
}

void get_word_length_distribution(const vector<Sample>& samples) {
  map<int, int> distribution;
  for (int sample_index = 0; sample_index < samples.size(); ++sample_index) {
    vector<string> words;
    split(samples[sample_index].original, words);
    for (int word_index = 0; word_index < words.size(); ++word_index) {
      distribution[words[word_index].size()]++; 
    }
  }
  cout << "word length " << endl << distribution;
}

void get_corrections_count_distribution(const vector<Sample>& samples) {
  map<int, int> distribution;
  for (int sample_index = 0; sample_index < samples.size(); ++sample_index) {
    distribution[samples[sample_index].corrections.size()]++; 
  }
  cout << "corrections count" << endl << distribution;
}

int get_trivial_corrections_count(const vector<Sample>& samples) {
  int trivial_count = 0;
  for (int sample_index = 0; sample_index < samples.size(); ++sample_index) {
    for (int correction_index = 0;
         correction_index < samples[sample_index].corrections.size();
         ++correction_index) {
      if (samples[sample_index].original ==
          samples[sample_index].corrections[correction_index]) {
        ++trivial_count;
      }
    }
  }
  return trivial_count;
}
 
int main() {
  vector<Sample> samples;
  Sample sample;
  while (cin >> sample) {
    samples.push_back(sample);
  }

  get_word_length_distribution(samples);
  get_corrections_count_distribution(samples);
  cout << "trivial corrections count "
       << 1.0 * get_trivial_corrections_count(samples) / samples.size() << endl;
  
  return 0;
}
