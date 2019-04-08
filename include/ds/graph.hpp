#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <functional>
#include <iomanip>
#include <iostream>
#include <queue>
#include <tuple>
#include <utility>
#include <vector>

#include "prettyprint.hpp"

using namespace std;

/*
 * Representation of graphs
 *
 * Vertices are represented by int
 */
namespace P {

int const INF = 0x3f3f3f3f;

struct Node {
  Node(int x) : val(x) {}
  Node() = default;
  friend ostream& operator<<(ostream& os, Node n) { return os << n.val; }
  bool operator==(Node const& n) const { return n.val == this->val; }
  bool operator!=(Node const& n) const { return !this->operator==(n); }
  operator int() const { return this->val; }

  int val;  // destination
};

struct WeightedNode {
  WeightedNode(int x) : val(x) {}
  WeightedNode(int x, int w) : val(x), w(w) {}
  WeightedNode() = default;
  friend ostream& operator<<(ostream& os, WeightedNode n) {
    return os << '(' << n.val << ", " << n.w << ')';
  }
  bool operator==(WeightedNode const& n) const {
    return n.val == this->val && n.w == this->w;
  }
  bool operator!=(WeightedNode const& n) const { return !this->operator==(n); }
  operator int() const { return this->val; }
  operator Node() const { return this->val; }

  int val;  // destination
  int w;    // weight
};

/*
 * Adjacency Matrix
 *
 * Can be used for Directed/Undirected graph
 *
 * Directed graph: (u -> v) is an edge <=> [u][v] > 0
 *
 * Space complexity: O(|V|^2)
 * Time complexity:
 * - Add/Remove edge: O(1)
 * - query adjacency: O(1)
 *
 */
struct AdjMat {
  AdjMat(int n) : N(max(n, 1)), mat(N * N) {}
  int& operator[](
      pair<int, int> p) {  // 2 vertices are connected by an edge ? (>=1) : 0
    auto [y, x] = p;
    return mat[y * N + x];  // mind the access pattern
  }
  int operator[](pair<int, int> p) const {
    auto [y, x] = p;
    return mat[y * N + x];
  }
  const int N;

  friend ostream& operator<<(ostream& os, AdjMat& mat) {
    mat.print(os);
    return os;
  }

  /*
   * Implicit step from path[N - 1] to path[0]
   */
  bool has_cycle(vector<Node> const& path) {
    const int M = path.size();
    for (int q = 0; q < M - 1; ++q)
      if ((*this)[{path[q], path[q + 1]}] == 0) return false;

    if ((*this)[{path[M - 1], path[0]}] == 0) return false;
    return true;
  }

  template <int PrintColWidth = 1>
  void print(ostream& os) const {
    for (int q = 0; q < N; ++q) {
      for (int w = 0; w < N - 1; ++w) {
        os << setw(PrintColWidth) << (*this)[{q, w}] << ", ";
      }
      os << setw(PrintColWidth) << (*this)[{q, N - 1}] << '\n';
    }
  }

 private:
  vector<int> mat;
};

/*
 * Adjacency List
 *
 * Can be used for Directed/Undirected graph
 *
 * Space complexity: O(|V| + |E|)
 * Time complexity:
 * - Add vertex: O(1)
 * - add edge: O(1)
 * - remove edge: O(|V|)
 * - query adjacency: O(|V|) for all the neighbors of a particular vertex
 *
 * To provide custom Dest Node, provide a conversion function to Node
 * and a conversion function to int (for destination)
 */
template <typename Dest = Node>
struct AdjList {
  AdjList(int n) : neigh(n) {}
  vector<Dest>& operator[](int x) { return neigh[x]; }
  vector<Dest> const& operator[](int x) const { return neigh[x]; }
  int N() const { return neigh.size(); }

  using iterator = typename vector<vector<Dest>>::iterator;
  using const_iterator = typename vector<vector<Dest>>::const_iterator;
  iterator begin() { return neigh.begin(); }
  iterator end() { return neigh.end(); }
  const_iterator begin() const { return cbegin(); }
  const_iterator end() const { return cend(); }
  const_iterator cbegin() const { return neigh.cbegin(); }
  const_iterator cend() const { return neigh.cend(); }

  friend ostream& operator<<(ostream& os, AdjList& al) {
    for (int q = 0; q < al.N(); ++q) {
      os << q << ": ";
      int M = al[q].size();
      for (int w = 0; w < M - 1; ++w) {
        os << al[q][w] << ", ";
      }
      if (!al[q].empty()) os << al[q][M - 1];
      os << '\n';
    }
    return os;
  }

  /*
   * For directed tree traversal from root, use dfs_dtree
   * to save the visited check
   *
   * Precondition: connected graph
   */
  template <typename F>
  void dfs(Node root, F f) const {
    vector<int> visited(N());
    auto dfs_ = [this, &visited, &f](Node node, auto dfs__) {
      if (visited[node]) return;
      visited[node] = true;
      f(node);
      for (auto v : neigh[node]) dfs__(v, dfs__);
    };
    dfs_(root, dfs_);
  }

  /*
   * Use only for directed tree traversal
   */
  template <typename F>
  void dfs_dtree(Node root, F f) const {
    auto dfs_ = [this, &f](Node node, auto dfs__) -> void {
      f(node);
      for (auto v : neigh[node]) dfs__(v, dfs__);
    };
    dfs_(root, dfs_);
  }

  template <typename F>
  void bfs(Node root, F f) const {
    queue<Node> q;
    q.push(root);
    vector<int> visited(N());
    visited[root] = true;
    while (!q.empty()) {
      auto u = q.front();
      q.pop();
      f(u);
      for (auto v : neigh[u]) {
        if (visited[v]) continue;
        visited[v] = true;
        q.push(v);
      }
    }
  }

  template <typename F>
  void bfs_depth(Node root, F f) const {
    queue<Node> q;
    q.push(root);
    vector<int> depth(N(), -1);
    depth[root] = 0;
    while (!q.empty()) {
      auto u = q.front();
      q.pop();
      f(u, depth[u]);
      for (auto v : neigh[u]) {
        if (depth[v] != -1) continue;
        depth[v] = depth[u] + 1;
        q.push(v);
      }
    }
  }

  /*
   * Time complexity: O(|V|+|E|)
   *
   * Precondition: graph is acyclic
   *
   * topological_sort_lex uses priority queue for lexicographically smallest
   * result
   */
  template <typename F>
  void topological_sort(F f) const {
    vector<int> in_deg(N());
    for (int u = 0; u < N(); ++u)
      for (auto v : neigh[u]) ++in_deg[v];
    queue<Node> q;
    for (int u = 0; u < N(); ++u)
      if (in_deg[u] == 0) q.push(u);

    while (!q.empty()) {
      auto u = q.front();
      q.pop();
      f(u);
      for (auto v : neigh[u])
        if (--in_deg[v] == 0) q.push(v);
    }
  }

  /*
   * Time complexity: O(|V|+|E|)
   * Space complexity: O(|V|)
   */
  bool is_topological_sort(vector<Node> const& seq) const {
    int M = seq.size();
    if (M != N()) return false;
    vector<int> pseq(N(), -1);  // permuted
    for (int q = 0; q < N(); ++q) {
      if (seq[q] >= N() || pseq[seq[q]] != -1) return false;
      pseq[seq[q]] = q;
    }
    for (int u = 0; u < N(); ++u)
      for (auto v : neigh[u])
        if (pseq[u] >= pseq[v]) return false;

    return true;
  }

  template <typename F>
  void topological_sort_lex(F f) const {
    vector<int> in_deg(N());
    for (int u = 0; u < N(); ++u)
      for (auto v : neigh[u]) ++in_deg[v];
    priority_queue<Node, vector<int>, greater<int>> q;
    for (int u = 0; u < N(); ++u)
      if (in_deg[u] == 0) q.push(u);

    while (!q.empty()) {
      auto u = q.top();
      q.pop();
      f(u);
      for (auto v : neigh[u])
        if (--in_deg[v] == 0) q.push(v);
    }
  }

  bool is_cyclic() const {
    vector<int> color(N());
    auto dfs_ = [this, &color](Node u, auto dfs__) -> bool {
      color[u] = 1;
      for (auto v : neigh[u])
        if (color[v] == 1 || (color[v] == 0 && dfs__(v, dfs__))) return true;

      color[u] = 2;
      return false;
    };
    for (int q = 0; q < N(); ++q)
      if (!color[q] && dfs_(q, dfs_)) return true;

    return false;
  }

  /*
   * Return a cycle with an implicit step from path[N - 1] to path[0]
   */
  vector<Node> find_cycle() const {
    vector<int> color(N());
    vector<Node> succ(N());  // successor
    pair<Node, Node> cycle_ends{0, 0};
    auto dfs_ = [this, &color, &succ, &cycle_ends](Node u, auto dfs__) -> bool {
      color[u] = 1;
      for (auto v : neigh[u])
        if (color[v] == 0) {
          succ[u] = v;
          if (dfs__(v, dfs__)) return true;
        } else if (color[v] == 1) {
          cycle_ends = {u, v};
          return true;
        }
      color[u] = 2;
      return false;
    };
    for (int q = 0; q < N(); ++q)
      if (!color[q] && dfs_(q, dfs_)) {
        auto [a, b] = cycle_ends;
        vector<Node> res;
        for (auto v = b; v != a; v = succ[v]) res.push_back(v);
        res.push_back(a);
        return res;
      }
    return {};
  }

  AdjMat to_adjmat() const {
    AdjMat mat(N());
    for (int q = 0; q < N(); ++q)
      for (int v : neigh[q]) mat[{q, v}] += 1;

    return mat;
  }

  bool has_cycle(vector<Node> const& path) const {
    return to_adjmat().has_cycle(path);
  }

  bool is_functional() const {
    return all_of(cbegin(), cend(),
                  [](auto const& adj_list) { return adj_list.size() < 2; });
  }

 private:
  vector<vector<Dest>> neigh;  // list of adjacent vertices
};

struct WeightedAdjList : public AdjList<WeightedNode> {
  using Base = AdjList<WeightedNode>;
  using Base::Base;

  /*
   * Precondition: There are no negative edges
   *
   * Time complexity: O(|E|log|V|)
   *
   * note 1: At most |E| duplicates of vertices so the complexity
   *            is still O(|E|log|V|)
   */
  vector<int> dijkstra(Node root) const {
    vector<int> dis(N(), INF);
    vector<int> done(N());
    dis[root] = 0;

    using E = pair<int, Node>;  // distance, node
    priority_queue<E, vector<E>, greater<E>> q;
    q.push({0, root});

    while (!q.empty()) {
      auto [_, u] = q.top(); // distance is smallest in q
      q.pop();
      if (done[u]) continue;
      done[u] = true;
      for (auto [v, w] : (*this)[u])
        if (dis[u] + w < dis[v])
          q.push({dis[v] = dis[u] + w, v});  // see note 1
    }
    return dis;
  }
};

/*
 * Functional graphs have at least 1 cycle
 */
struct FunctionalGraph {
  FunctionalGraph(int n) : succ(n) {}
  int& operator[](int x) { return succ[x]; }
  int N() const { return succ.size(); }

  /*
   * Find one of the cycles in the functional graph
   */
  vector<Node> find_cycle(int x = 0) const {
    int a = x, b = x;
    do {
      a = succ[a];
      b = succ[succ[b]];
    } while (a != b);
    vector<Node> res;
    do {
      res.push_back(a);
      a = succ[a];
    } while (a != b);
    return res;
  }

  /*
   * Implicit step from path[N - 1] to path[0]
   */
  bool has_cycle(vector<Node> const& path) const {
    const int M = path.size();
    if (M == 0 || succ[path[M - 1]] != path[0]) return false;

    for (int q = 0; q < M - 1; ++q)
      if (succ[path[q]] != path[q + 1]) return false;
    return true;
  }

 private:
  vector<int> succ;
};

/*
 * Incidence Matrix
 *
 * Too slow for use for most cases
 */
}  // namespace P
#endif /* GRAPH_HPP */
