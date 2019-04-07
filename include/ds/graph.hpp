#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <functional>
#include <iomanip>
#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

using namespace std;

/*
 * Representation of graphs
 *
 * Vertices are represented by int
 */
namespace P {

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
  bool has_cycle(vector<int> const& path) {
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
 */
struct AdjList {
  AdjList(int n) : neigh(n) {}
  vector<int>& operator[](int x) { return neigh[x]; }
  int N() const { return neigh.size(); }

  using iterator = vector<vector<int>>::iterator;
  using const_iterator = vector<vector<int>>::const_iterator;
  iterator begin() { return neigh.begin(); }
  iterator end() { return neigh.end(); }
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
   * Assumes connected graph
   */
  template <typename F>
  void dfs(int root, F f) const {
    vector<int> visited(N());
    auto dfs_ = [this, &visited, &f](int node, auto dfs__) {
      if (visited[node]) return;
      visited[node] = true;
      f(node);
      for (int v : neigh[node]) dfs__(v, dfs__);
    };
    dfs_(root, dfs_);
  }

  /*
   * Use only for directed tree traversal
   */
  template <typename F>
  void dfs_dtree(int root, F f) const {
    auto dfs_ = [this, &f](int node, auto dfs__) -> void {
      f(node);
      for (int v : neigh[node]) dfs__(v, dfs__);
    };
    dfs_(root, dfs_);
  }

  template <typename F>
  void bfs(int root, F f) const {
    queue<int> q;
    q.push(root);
    vector<int> visited(N());
    visited[root] = true;
    while (!q.empty()) {
      int u = q.front();
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
  void bfs_depth(int root, F f) const {
    queue<int> q;
    q.push(root);
    vector<int> depth(N());
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      f(u, depth[u]);
      for (auto v : neigh[u]) {
        if (depth[v]) continue;
        depth[v] = depth[u] + 1;
        q.push(v);
      }
    }
  }

  /*
   * Time complexity: O(|V|+|E|)
   *
   * Assumes the graph is acyclic
   *
   * topological_sort_lex uses priority queue for lexicographically smallest
   * result
   */
  template <typename F>
  void topological_sort(F f) const {
    vector<int> in_deg(N());
    for (int u = 0; u < N(); ++u)
      for (int v : neigh[u]) ++in_deg[v];
    queue<int> q;
    for (int u = 0; u < N(); ++u)
      if (in_deg[u] == 0) q.push(u);

    while (!q.empty()) {
      int u = q.front();
      q.pop();
      f(u);
      for (int v : neigh[u])
        if (--in_deg[v] == 0) q.push(v);
    }
  }

  /*
   * Time complexity: O(|V|+|E|)
   * Space complexity: O(|V|)
   */
  bool is_topological_sort(vector<int> const& seq) const {
    int M = seq.size();
    if (M != N()) return false;
    vector<int> pseq(N(), -1);  // permuted
    for (int q = 0; q < N(); ++q) {
      if (seq[q] >= N() || pseq[seq[q]] != -1) return false;
      pseq[seq[q]] = q;
    }
    for (int u = 0; u < N(); ++u)
      for (int v : neigh[u])
        if (pseq[u] >= pseq[v]) return false;

    return true;
  }

  template <typename F>
  void topological_sort_lex(F f) const {
    vector<int> in_deg(N());
    for (int u = 0; u < N(); ++u)
      for (int v : neigh[u]) ++in_deg[v];
    priority_queue<int, vector<int>, greater<int>> q;
    for (int u = 0; u < N(); ++u)
      if (in_deg[u] == 0) q.push(u);

    while (!q.empty()) {
      int u = q.top();
      q.pop();
      f(u);
      for (int v : neigh[u])
        if (--in_deg[v] == 0) q.push(v);
    }
  }

  bool is_cyclic() const {
    vector<int> color(N());
    auto dfs_ = [this, &color](int u, auto dfs__) -> bool {
      color[u] = 1;
      for (int v : neigh[u])
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
  vector<int> find_cycle() const {
    vector<int> color(N());
    vector<int> succ(N());  // successor
    pair<int, int> cycle_ends;
    auto dfs_ = [this, &color, &succ, &cycle_ends](int u, auto dfs__) -> bool {
      color[u] = 1;
      for (int v : neigh[u])
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
        vector<int> res;
        for (int v = b; v != a; v = succ[v]) res.push_back(v);
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

  bool has_cycle(vector<int> const& path) const {
    return to_adjmat().has_cycle(path);
  }

  bool is_functional() const {
    return all_of(cbegin(), cend(),
                  [](auto const& adj_list) { return adj_list.size() < 2; });
  }

 private:
  vector<vector<int>> neigh;  // list of adjacent vertices
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
  vector<int> find_cycle(int x = 0) const {
    int a = x, b = x;
    do {
      a = succ[a];
      b = succ[succ[b]];
    } while (a != b);
    vector<int> res;
    do {
      res.push_back(a);
      a = succ[a];
    } while (a != b);
    return res;
  }

  /*
   * Implicit step from path[N - 1] to path[0]
   */
  bool has_cycle(vector<int> const& path) const {
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
