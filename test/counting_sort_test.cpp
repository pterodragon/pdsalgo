#include <catch2/catch.hpp>

#include <algorithm>
#include <iostream>
#include "algo/counting_sort.hpp"

#include "prettyprint.hpp"

using namespace P;
TEST_CASE("counting sort inplace", "[counting_sort]") {
  string s = GENERATE(as<string>{},
                      "",      //
                      "a",     //
                      "z",     //
                      "aa",    //
                      "aaa",   //
                      "abab",  //
                      "hellozzaz");
  DYNAMIC_SECTION("s : " << s) {
    string exp = s;
    stable_sort(begin(exp), end(exp));
    vector sv(begin(s), end(s));
    counting_sort_inplace(sv);
    string actual(begin(sv), end(sv));
    REQUIRE(actual == exp);
  }
}

TEST_CASE("counting sort", "[counting_sort]") {
  vector<string> vstr(27, "aaa");
  for (int q = 0; q < 3; ++q)
    for (int w = 0; w < 3; ++w)
      for (int e = 0; e < 3; ++e)
        vstr[q + w * 3 + e * 9][0] += q, vstr[q + w * 3 + e * 9][1] += w,
            vstr[q + w * 3 + e * 9][2] += e;
  vector<string> vexp = vstr;
  // radix sort
  for (int i = 0; i <= 2; ++i) {
    DYNAMIC_SECTION("sorting all permutations by str[" << i << "]") {
      vstr = counting_sort(vstr, [i](auto const& str) { return str[i]; });
      stable_sort(begin(vexp), end(vexp),
                  [i](string s1, string s2) { return s1[i] < s2[i]; });
      REQUIRE(vstr == vexp);
    }
  }
  for (int i = 2; i >= 0; --i) {
    DYNAMIC_SECTION("sorting all permutations by str[" << i << "]") {
      vstr = counting_sort(vstr, [i](auto const& str) { return str[i]; });
      stable_sort(begin(vexp), end(vexp),
                  [i](string s1, string s2) { return s1[i] < s2[i]; });
      REQUIRE(vstr == vexp);
    }
  }
}
