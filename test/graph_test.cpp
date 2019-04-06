#include <catch2/catch.hpp>

#include <cmath>
#include <iostream>

#include "ds/graph.hpp"
#include "prettyprint.hpp"

using namespace P;
using namespace std;

TEST_CASE("graph traversal", "[graph]") {
  auto n = GENERATE(1, 2, 15, 16, 17, 37);
  vector<int> seq;
  {
    AdjList al(n);
    for (int y = 0; y < n; ++y) {
      for (int x = 0; x < n; ++x) {
        if (abs(y - x) == 1 || (y + x) % 3 == 0) al[y].push_back(x);
      }
    }
    for (int q = 0; q < n; ++q) {
      al.dfs(q, [&seq](int x) { seq.push_back(x); });
      REQUIRE(seq.size() == n);
      seq.clear();
      al.bfs(q, [&seq](int x) { seq.push_back(x); });
      REQUIRE(seq.size() == n);
      seq.clear();
    }
    seq.clear();
  }

  {
    if (n > 2) {
      AdjList al(n);
      for (int y = 0; y < n; ++y) {
        for (int x = 0; x < n; ++x) {
          if (abs(y - x) == 3) al[y].push_back(x);
        }
      }
      int root = 2;
      al.dfs(root, [&seq](int x) { seq.push_back(x); });
      vector<int> exp;
      for (int q = root; q < n; q += 3) exp.push_back(q);
      REQUIRE(seq == exp);
      seq.clear();
    }
  }
  {
    AdjList al(n);
    for (int y = 0; y < n; ++y) {
      for (int x = 0; x < n; ++x) {
        if (y != x && (x / 2 == y)) al[y].push_back(x);
      }
    }  // complete binary tree (directed from root)
    al.dfs_dtree(0, [&seq](int x) { seq.push_back(x); });
    REQUIRE(seq.size() == n);
    seq.clear();
    al.bfs(0, [&seq](int x) { seq.push_back(x); });
    REQUIRE(seq.size() == n);
    seq.clear();
    vector<int> exp_depth;
    vector<int> actual_depth;
    for (int q = 0; q < n; ++q) {
      exp_depth.push_back(q ? sizeof(q) * 8 - __builtin_clz(q) : 0);
    }
    al.bfs_depth(0, [&seq, &actual_depth](int x, int depth) {
      seq.push_back(x);
      actual_depth.push_back(depth);
    });
    REQUIRE(seq.size() == n);
    REQUIRE(actual_depth == exp_depth);
    seq.clear();
  }
}
