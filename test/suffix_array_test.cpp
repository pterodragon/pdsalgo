#include <catch2/catch.hpp>

#include "ds/suffix_array.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <unordered_set>

#include "prettyprint.hpp"

using namespace P;
using namespace std;

vector<string> to_sorted_suffixes(string x) {
  vector<string> res;
  for (int q = 0; q < (int)x.size(); ++q) res.push_back(x.substr(q));
  sort(begin(res), end(res));

  return res;
}

TEST_CASE("suffix array build & method tests2", "[suffix_array]") {
  SECTION("1") {
    string s = "babaabaaabaaaabaaaa{aaabba";
    SuffixArray<27, 'a'> sa(s);
    auto exp = to_sorted_suffixes(s);
    auto vec = sa.to_vector();
    CAPTURE(vec);
    REQUIRE(vec == exp);
  }
  SECTION("2") {
    string s = "aaabba"s + '{' + "babaabaaabaaaabaaaa"s;
    SuffixArray<27, 'a'> sa(s);
    auto exp = to_sorted_suffixes(s);
    auto vec = sa.to_vector();
    CAPTURE(vec);
    CAPTURE(exp);
    REQUIRE(vec == exp);
  }
}

TEST_CASE("suffix array build & method tests", "[suffix_array]") {
  string s =
      GENERATE(as<std::string>{}, "abcabcdcabx", "aaaaa",
               "babaabaaabaaaabaaaaa", "", "aababcabcdabcde", "aaabaabaaa");
  SuffixArray sa(s);
  auto exp = to_sorted_suffixes(s);
  auto vec = sa.to_vector();
  REQUIRE(vec == exp);
  int N = s.size();
  for (int q = 0; q < N; ++q) {
    for (int w = 1; q + w < N; ++w) {
      unordered_set<string_view> set;
      string_view sv(s.data() + q, w);
      if (set.find(sv) != end(set)) continue;
      set.insert(sv);
      DYNAMIC_SECTION("binary_search; s = " << s << ", pat = " << sv) {
        auto v = sa.binary_search(sv);
        for (auto e : v) {
          string_view res(s.data() + e, sv.size());
          REQUIRE(res == sv);
        }
      }
    }
  }
  DYNAMIC_SECTION("lcp; s = " << s) {
    // compute lcp by naive method
    vector<string> strs;
    for (int q = 0; q < N; ++q) {
      strs.push_back(s.substr(q));
    }
    sort(begin(strs), end(strs));
    vector<int> lcp;
    if (!s.empty()) lcp.push_back(0);
    for (int q = 1; q < (int)s.size(); ++q) {
      auto str1 = strs[q - 1];
      auto str2 = strs[q];
      lcp.push_back(distance(
          begin(str1), mismatch(begin(str1), end(str1), begin(str2)).first));
    }
    auto actual_lcp = sa.lcp();
    REQUIRE(actual_lcp == lcp);
  }
}

TEST_CASE("suffix array lcs", "[suffix_array]") {
  using T = array<string_view, 3>;
  auto examples = GENERATE(                         //
      as<T>{},                                      //
      T{"gatagaca", "cata", "ata"},                 //
      T{"jdfkal", "vq", ""},                        //
      T{"aaa", "aaa", "aaa"},                       //
      T{"c", "aabbbdeeefffff", ""},                 //
      T{"ba", "xxxa", "a"},                         //
      T{"babaabaaabaaaabaaaa", "aaabba", "aaab"},   //
      T{"babaabaaabaaaabaaaaa", "aaaaa", "aaaaa"},  //
      T{"babaabaaabaaaabaaaaa", "", ""},            //
      T{"", "", ""},                                //
      T{"xxxa", "ba", "a"});
  auto [s1, s2, exp] = examples;
  DYNAMIC_SECTION("lcs; s1 = " << s1 << ", s2 = " << s2) {
    auto actual = SuffixArray<>::lcs(s1, s2);
    REQUIRE(actual == exp);
    actual = SuffixArray<>::lcs(s2, s1);
    REQUIRE(actual == exp);
    actual = SuffixArray<>::lcs<'|'>(s2, s1);
    REQUIRE(actual == exp);
    actual = SuffixArray<>::lcs<'|'>(s1, s2);
    REQUIRE(actual == exp);
    actual = SuffixArray<27, 'a'>::lcs<'{'>(s2, s1);
    REQUIRE(actual == exp);
    actual = SuffixArray<27, 'a'>::lcs<'{'>(s1, s2);
    REQUIRE(actual == exp);
    actual = SuffixArray<256>::lcs<'\xff'>(s2, s1);
    REQUIRE(actual == exp);
    actual = SuffixArray<256>::lcs<'\xff'>(s1, s2);
    REQUIRE(actual == exp);
  }
}
