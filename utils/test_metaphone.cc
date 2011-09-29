#include <using_std.h>
#include <dmetaph.h>

int main() {

  while(!cin.eof()) {
    string word;
    cin >> word;
    if (word.empty()) {
      continue;
    }
    cout << metaphone(word) << endl;
  }

  return 0;
}
