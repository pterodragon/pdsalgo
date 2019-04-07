#include <catch2/catch.hpp>

#include <cmath>
#include <iostream>
#include <random>

#include "ds/graph.hpp"

using namespace P;
using namespace std;
TEST_CASE("dijkstra's algorithm", "[graph]") {
  int n = 6;
  WeightedAdjList al(n);
  auto add_edge = [&al](auto u, auto v, int w) {
    al[u].push_back({v, w});
    al[v].push_back({u, w});
  };
  add_edge(1, 2, 5);
  add_edge(1, 4, 9);
  add_edge(1, 5, 1);
  add_edge(4, 5, 2);
  add_edge(4, 3, 6);
  add_edge(2, 3, 2);
  CAPTURE(al);
  SECTION("isolated node") {
    auto d = al.dijkstra(0);
    CAPTURE(d);
    vector exp{0, INF, INF, INF, INF, INF};
    REQUIRE(d == exp);
  }
  SECTION("reducible case") {
    auto d = al.dijkstra(1);
    CAPTURE(d);
    vector exp{INF, 0, 5, 7, 3, 1};
    REQUIRE(d == exp);
  }
}

TEST_CASE("weighted graph", "[graph]") {
  int n = 64;
  WeightedAdjList al(n);
  al[0].push_back({1, 2});
  al[1].push_back({0, 2});

  SECTION("dfs") {
    vector<Node> seq;
    al.dfs(0, [&seq](Node x) { seq.push_back(x); });
    vector<Node> exp{0, 1};
    REQUIRE(seq == exp);
  }
  SECTION("bfs") {
    vector<Node> seq;
    al.bfs(0, [&seq](Node x) { seq.push_back(x); });
    vector<Node> exp{0, 1};
    REQUIRE(seq == exp);
    seq.clear();
    al.bfs_depth(0, [&seq](Node x, int depth) { seq.push_back(x); });
    REQUIRE(seq == exp);
  }
  SECTION("cyclic") { REQUIRE(al.is_cyclic()); }
}

TEST_CASE("weighted graph2", "[graph]") {
  int n = 64;
  WeightedAdjList al(n);
  for (int y = 0; y < n; ++y) {
    for (int x = 0; x < n; ++x) {
      if (y != x && (x / 2 == y)) al[y].push_back(x);
    }
  }  // complete binary tree (directed from root)
  vector<Node> seq;
  al.dfs_dtree(0, [&seq](Node x) { seq.push_back(x); });
  seq.clear();
  al.topological_sort_lex([&seq](Node x) { seq.push_back(x); });
  REQUIRE(al.is_topological_sort(seq));
  seq.clear();
  al.topological_sort([&seq](Node x) { seq.push_back(x); });
  REQUIRE(al.is_topological_sort(seq));
}

TEST_CASE("graph traversal", "[graph]") {
  auto n = GENERATE(1, 2, 15, 16, 17, 37);
  vector<Node> seq;
  {
    AdjList al(n);
    for (int y = 0; y < n; ++y) {
      for (int x = 0; x < n; ++x) {
        if (abs(y - x) == 1 || (y + x) % 3 == 0) al[y].push_back(x);
      }
    }
    for (int q = 0; q < n; ++q) {
      al.dfs(q, [&seq](Node x) { seq.push_back(x); });
      REQUIRE(seq.size() == n);
      seq.clear();
      al.bfs(q, [&seq](Node x) { seq.push_back(x); });
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
      al.dfs(root, [&seq](Node x) { seq.push_back(x); });
      vector<Node> exp;
      for (int q = root; q < n; q += 3) exp.push_back(Node(q));
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
    al.dfs_dtree(0, [&seq](Node x) { seq.push_back(x); });
    REQUIRE(seq.size() == n);
    seq.clear();
    al.bfs(0, [&seq](Node x) { seq.push_back(x); });
    REQUIRE(seq.size() == n);
    seq.clear();
    vector<int> exp_depth;
    vector<int> actual_depth;
    for (int q = 0; q < n; ++q) {
      exp_depth.push_back(q ? sizeof(q) * 8 - __builtin_clz(q) : 0);
    }
    al.bfs_depth(0, [&seq, &actual_depth](Node x, int depth) {
      seq.push_back(x);
      actual_depth.push_back(depth);
    });
    REQUIRE(seq.size() == n);
    REQUIRE(actual_depth == exp_depth);
    seq.clear();
  }
}

TEST_CASE("topological sort lexicographical", "[graph]") {
  const int n = 6;
  AdjList al(n);
  al[5].push_back(2);
  al[5].push_back(0);
  al[4].push_back(0);
  al[4].push_back(1);
  al[2].push_back(3);
  al[3].push_back(1);
  vector<Node> seq;
  al.topological_sort_lex([&seq](Node x) { seq.push_back(x); });
  vector<Node> exp{4, 5, 0, 2, 3, 1};
  REQUIRE(seq == exp);
  REQUIRE(al.is_topological_sort(seq));
}

TEST_CASE("topological sort", "[graph]") {
  const int n = GENERATE(0, 1, 2, 3, 4, 33, 128);
  mt19937 gen;
  uniform_int_distribution<> dis(0, n - 1);

  AdjList al(n);
  for (int q = 0; q < 3 * n; ++q) {
    Node a = dis(gen);
    Node b = dis(gen);
    if (find(cbegin(al[a]), cend(al[a]), b) == cend(al[a])) {
      al[a].push_back(b);
      if (al.is_cyclic()) al[a].pop_back();
    }
  }
  vector<Node> seq;
  al.topological_sort([&seq](Node x) { seq.push_back(x); });
  CAPTURE(seq);
  REQUIRE(al.is_topological_sort(seq));
}

TEST_CASE("is cyclic", "[graph]") {
  SECTION("cyclic case") {
    AdjList al(1);
    auto after_check = [&al]() {
      REQUIRE(al.is_cyclic());
      auto cycle = al.find_cycle();
      REQUIRE(al.has_cycle(cycle));
    };
    SECTION("self loop") {
      al = AdjList(5);
      al[3].push_back(3);
      after_check();
    }
    SECTION("large loop") {
      const int n = 4;
      al = AdjList(n);
      al[0].push_back(1);
      al[1].push_back(2);
      al[2].push_back(3);
      al[3].push_back(0);
      after_check();
    }
  }
  SECTION("tree case") {
    const int n = GENERATE(4, 15, 33, 129);
    AdjList al(n);
    for (int q = 1; q * 2 + 1 < n; ++q) {
      al[q].push_back(q * 2);
      al[q].push_back(q * 2 + 1);
    }
    REQUIRE_FALSE(al.is_cyclic());
    al[n / 2].push_back(n / 4);
    REQUIRE(al.is_cyclic());
  }
}

TEST_CASE("is functional", "[graph]") {
  const int n = GENERATE(0, 1, 2, 3, 4, 33, 128);
  CAPTURE(n);
  mt19937 gen;
  uniform_int_distribution<> dis(0, n - 1);

  AdjList al(n);
  for (auto& v : al)
    if (gen() % 2) v.push_back(dis(gen));

  REQUIRE(al.is_functional());
  for (auto& v : al) v.push_back(dis(gen));
  for (auto& v : al) v.push_back(dis(gen));
  if (n > 0) REQUIRE_FALSE(al.is_functional());
}

TEST_CASE("functional graph find cycle", "[graph]") {
  SECTION("self loop") {
    const int n = 1;
    FunctionalGraph g(n);
    g[0] = 0;
    auto res = g.find_cycle();
    REQUIRE(g.has_cycle(res));
    REQUIRE(res.size() == 1);
  }
  SECTION("large loop") {
    const int n = 4;
    FunctionalGraph g(n);
    g[0] = 1;
    g[1] = 2;
    g[2] = 3;
    g[3] = 0;
    auto res = g.find_cycle();
    REQUIRE(g.has_cycle(res));
    REQUIRE(res.size() == 4);
  }
  SECTION("go straight and enter loop") {
    const int n = 4;
    FunctionalGraph g(n);
    g[0] = 1;
    g[1] = 2;
    g[2] = 3;
    g[3] = 2;
    auto res = g.find_cycle();
    REQUIRE(g.has_cycle(res));
    REQUIRE(res.size() == 2);
  }
}
