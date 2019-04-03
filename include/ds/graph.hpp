#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iomanip>
#include <iostream>
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
