#pragma once
#include <using_std.h>

// Data in Speelling.Learn.dat
struct Sample {
  string original;
  vector<CorrectionCandidate> corrections;
};

std::istream& operator>>(std::istream& in, Sample& sample) {
  sample.original.clear();
  sample.corrections.clear();

  vector<string> fields;
  boost::split(fields, line, boost::is_any_of("\t"));
  assert(fields.size() > 1);
  original = fields[0];
  for (int i = 1; i < fields.size(); ++i) {
    corrections[i].text_ = fields[i];
    correction[i].weight_ = 0;
  }
  
  return in;
}

std::ostream& operator<<(std::ostream& out, const Sample& sample) {
  out << sample.original;
  for (int i = 0; i < sample.corrections.size(); ++i) {
    out << "\t";
    if (sample.corrections_[i].weight != 0.0) {
      out << ":" << sample.corrections_[i].weight_;
    }
    out << sample.corrections_[i].text_;
  }
  return out;
}
