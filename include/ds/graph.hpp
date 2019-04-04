#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iomanip>
#include <iostream>
#include <tuple>
#include <vector>
#include <queue>

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
  void dfs(int root, F f) {
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
  void dfs_dtree(int root, F f) {
    auto dfs_ = [this, &f](int node, auto dfs__) -> void {
      f(node);
      for (int v : neigh[node]) dfs__(v, dfs__);
    };
    dfs_(root, dfs_);
  }

  template <typename F>
  void bfs(int root, F f) {
    queue<int> q;
    q.push(root);
    vector<int> visited(N());
    visited[root] = true;
    while (!q.empty()) {
      int s = q.front();
      q.pop();
      f(s);
      for (auto v: neigh[s]) {
        if (visited[v]) continue;
        visited[v] = true;
        q.push(v);
      }
    }
  }

  template <typename F>
  void bfs_depth(int root, F f) {
    queue<int> q;
    q.push(root);
    vector<int> depth(N());
    while (!q.empty()) {
      int s = q.front();
      q.pop();
      f(s, depth[s]);
      for (auto v: neigh[s]) {
        if (depth[v]) continue;
        depth[v] = depth[s] + 1;
        q.push(v);
      }
    }
  }
 private:
  vector<vector<int>> neigh;  // list of adjacent vertices
};

/*
 * Incidence Matrix
 *
 * Too slow for use for most cases
 */
}  // namespace P
#endif /* GRAPH_HPP */
