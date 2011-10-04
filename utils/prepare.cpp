#include <iostream>
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;

using std::vector;
using std::string;

int main() {
  while(!cin.eof()) {
    vector<string> fields;
    string line;
    std::getline(cin, line);

    boost::split(fields, line, boost::is_any_of("\t\n\r"));
    if (sields.size() < 5) {
      continue;
    }
    if (fields[0].find_first_not_of("abcdefghijklmnopqrstuvwxyz ") != std::string::npos) {
      continue;
    }

    cout 
      << fields[2] << "\t"
      << fields[0] << "\t"
      << fields[4] << endl;
  
  }

  return 0;
}
