#include <gtest/gtest.h>

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>

#include <trie.h>

using std::cout;
using std::endl;
using std::copy;
using std::ostream_iterator;
using std::back_inserter;
using std::ostringstream;
using std::sort;
using std::vector;

template <typename TData>
string find_terms_in_trie(TTrie<TData>& trie, const string& text, int max_distance) {
    vector<TData> out_vec;
    trie.find_nearby(text, back_inserter(out_vec), max_distance);
    
    sort(out_vec.begin(), out_vec.end());
    ostringstream out_stream;
    copy(out_vec.begin(), out_vec.end(), ostream_iterator<TData>(out_stream, " "));
    
    string out;
    out = out_stream.str();
    return out.substr(0, out.size() - 1);
}

TEST(TTrie_Test,StoreStrings) {
    TTrie<string> trie;

    trie.add("aaaaaa", "aaaaaa");
    trie.add("aaabaa", "aaabaa");
    trie.add("aabaaa", "aabaaa");
    trie.add("abaaaa", "abaaaa");

    EXPECT_EQ(trie.find("aaaaaa"), "aaaaaa");
    EXPECT_EQ(find_terms_in_trie(trie, "aaaaaa", 0), "aaaaaa");
    EXPECT_EQ(find_terms_in_trie(trie, "aaaaaa", 1), "aaaaaa aaabaa aabaaa abaaaa");
}

TEST(TTrie_Test,Add_or_Find_Test) {
    TTrie<int> trie;

    trie.add("aaa", 1);
    trie.add("bba", 2);
    trie.add("aab", 3);

    EXPECT_EQ(trie.find("aaa"), 1);
    EXPECT_EQ(trie.find("bba"), 2);
    EXPECT_EQ(trie.find("aab"), 3);
    
    EXPECT_EQ(find_terms_in_trie(trie, "aaa", 0), "1");
    EXPECT_EQ(find_terms_in_trie(trie, "aab", 1), "1 3");
    EXPECT_EQ(find_terms_in_trie(trie, "bba", 0), "2");
    
    // miss single letter
    trie.add("bcdefgh", 11);
    trie.add("acdefgh", 12);
    trie.add("abdefgh", 13);
    trie.add("abcefgh", 14);
    trie.add("abcdfgh", 15);
    trie.add("abcdegh", 16);
    trie.add("abcdefh", 17);
    trie.add("abcdefg", 18);
    EXPECT_EQ(find_terms_in_trie(trie, "abcdefgh", 0), "");
    EXPECT_EQ(find_terms_in_trie(trie, "abcdefgh", 1), "11 12 13 14 15 16 17 18");

    trie.add("abacabaca", 20);
    // neighbour transposition (eq double change)
    EXPECT_EQ(find_terms_in_trie(trie, "abaacbaca", 0), "");
    EXPECT_EQ(find_terms_in_trie(trie, "abaacbaca", 1), "");
    EXPECT_EQ(find_terms_in_trie(trie, "abaacbaca", 2), "20");
    EXPECT_EQ(find_terms_in_trie(trie, "abaacbaca", 3), "20");
    EXPECT_EQ(find_terms_in_trie(trie, "abaacbaca", 4), "20");
   
    // double change
    EXPECT_EQ(find_terms_in_trie(trie, "aBacabaCa", 0), "");
    EXPECT_EQ(find_terms_in_trie(trie, "aBacabaCa", 1), "");
    EXPECT_EQ(find_terms_in_trie(trie, "aBacabaCa", 2), "20");
    EXPECT_EQ(find_terms_in_trie(trie, "aBacabaCa", 3), "20");
    EXPECT_EQ(find_terms_in_trie(trie, "aBacabaCa", 4), "20");
    
    // add extra symbol
    EXPECT_EQ(find_terms_in_trie(trie, "abacCabaca", 0), "");
    EXPECT_EQ(find_terms_in_trie(trie, "abacCabaca", 1), "20");
    EXPECT_EQ(find_terms_in_trie(trie, "abacCabaca", 2), "20");

    EXPECT_EQ(find_terms_in_trie(trie, "abacCDabaca", 0), "");
    EXPECT_EQ(find_terms_in_trie(trie, "abacCDabaca", 1), "");
    EXPECT_EQ(find_terms_in_trie(trie, "abacCDabaca", 2), "20");
}
