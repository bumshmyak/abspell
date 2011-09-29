#pragma once

#include <stdafx.h>

class MString : public CString {
 public:
  MString();
  MString(const char* in);
  MString(const CString& in);
  bool SlavoGermanic();
  inline void MetaphAdd(const char* main);
  inline void MetaphAdd(const char* main, const char* alt); 
  bool IsVowel(int at);
  bool StringAt(int start, int length, ... );
  void DoubleMetaphone(CString &metaph, CString &metaph2);

  CString primary;
  CString secondary;
  bool alternate;
  size_t length;
  int last;
};

string metaphone(const string& word) {
  CString metaph;
  CString metaph2;
  MString(word.c_str()).DoubleMetaphone(metaph, metaph2);
  return metaph.data;
}

/* // returns some stuff
string metaphone2(const string& word) {
  CString metaph;
  CString metaph2;
  MString(word.c_str()).DoubleMetaphone(metaph, metaph2);
  return metaph2.data;
}
*/
