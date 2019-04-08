#include <catch2/catch.hpp>

#include "ds/suffix_tree.hpp"

#include <cassert>
#include <unordered_set>

#include "prettyprint.hpp"

using namespace P;
using namespace std;

TEST_CASE("suffix tree build & method tests", "[suffix_tree]") {
  string s =
      GENERATE(as<std::string>{}, "abcabcdcabx", "aaaaa",
               "babaabaaabaaaabaaaaa", "", "aababcabcdabcde", "aaabaabaaa");
  DYNAMIC_SECTION("has_substr; s = " << s) {
    SuffixTree<27, 'a'> st(s);
    for (int q = 0; q < st.N; ++q) {
      for (int w = 1; q + w < st.N + 1; ++w) {
        string_view pat(s.data() + q, w);
        REQUIRE(st.has_substr(pat));
        string fail_s = string(pat) + string(1, 'a' + 27);
        REQUIRE_FALSE(st.has_substr(fail_s));
        string fail_s2 = string(pat.substr(0, pat.size() / 2)) +
                         string(1, 'a' + 27) +
                         string(pat.substr(pat.size() / 2));
        REQUIRE_FALSE(st.has_substr(fail_s2));
      }
    }
    REQUIRE(st.has_substr(""));
  }
  DYNAMIC_SECTION("lcs; s = " << s) {
    string uc(1, 'a' + 27);
    int const N = s.size();
    for (int q = 0; q < N; ++q) {
      for (int w = q + 1; q + w < N; ++w) {
        string_view pat(s.data() + q, w);
        auto arg = string(pat);
        auto lcs = SuffixTree<29, 'a'>::lcs(s, arg);
        REQUIRE(lcs == pat);
        arg = uc + string(pat);
        lcs = SuffixTree<29, 'a'>::lcs(s, arg);
        REQUIRE(lcs == pat);
      }
    }
    auto lcs = SuffixTree<29, 'a'>::lcs(s, uc);
    REQUIRE(lcs == "");
  }
  DYNAMIC_SECTION("search all; s = " << s) {
    string y = s + string(1, 'a' + 26);  // append unique char
    int const N = y.size();
    SuffixTree<27, 'a'> st(y);
    for (int q = 0; q < N; ++q) {
      for (int w = 1; q + w < N; ++w) {
        string_view pat(s.data() + q, w);
        unordered_set<int> exp;
        for (int e = 0; e + pat.size() <= y.size(); ++e)
          if (pat == s.substr(e, pat.size())) exp.insert(e);

        auto v = st.search_all(pat);
        unordered_set<int> actual(begin(v), end(v));
        REQUIRE(actual == exp);
      }
    }
  }
  DYNAMIC_SECTION("lrs; s = " << s) {
    string y = s + string(1, 'a' + 26);  // append unique char
    SuffixTree<27, 'a'> st(y);
    int N = y.size();
    // compute lrs by dynamic programming
    vector<vector<int>> dp(N, vector<int>(N));
    // dp[i][j]: longest of suffix of s[i:j+1] which is repeating in s
    pair<int, int> res;  // len, end idx
    for (int q = 1; q < N; ++q) {
      for (int w = q + 1; w < N; ++w) {
        dp[q][w] = s[q - 1] == s[w - 1] ? dp[q - 1][w - 1] + 1 : 0;
        res = max(res, {dp[q][w], w});
      }
    }
    auto [len, res_w] = res;
    auto res_sv = s.substr(res_w - len, len);
    REQUIRE(st.lrs() == res_sv);
    REQUIRE(st.lrs_dfs() == res_sv);
  }
}
