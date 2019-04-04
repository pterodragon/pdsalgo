#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

#include "ds/graph.hpp"
#include "prettyprint.hpp"

using namespace P;
using namespace std;

#define testcase(res, exp)                  \
  {                                         \
    cout << #res << " = " << res << '\t';   \
    bool ok = ((res) == (exp));             \
    cout << " -- " << (ok ? "OK" : "FAIL"); \
    if (!ok) cout << "; exp = " << exp;     \
    cout << '\n';                           \
    cout.flush();                           \
    assert(ok);                             \
  }

void test_traversal() {
  for (int n : {1, 2, 15, 16, 17, 37}) {
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
        testcase(seq.size(), n);
        seq.clear();
        al.bfs(q, [&seq](int x) { seq.push_back(x); });
        testcase(seq.size(), n);
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
        testcase(seq, exp);
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
      cout << al << '\n';
      cout.flush();
      al.dfs_dtree(0, [&seq](int x) { seq.push_back(x); });
      testcase(seq.size(), n);
      seq.clear();
      al.bfs(0, [&seq](int x) { seq.push_back(x); });
      testcase(seq.size(), n);
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
      testcase(seq.size(), n);
      testcase(actual_depth, exp_depth);
      seq.clear();
    }
  }
}

int main(int argc, char** argv) {
  int n = argv[1] ? stoi(string(argv[1])) : 10;
  AdjMat m(n);
  for (int y = 0; y < n; ++y) {
    for (int x = 0; x < n; ++x) {
      m[{y, x}] = (y + x) % 7 == 0 || x % 3 == 0 ? 111 : 0;
    }
  }
  m.print<4>(cout);
  cout << '\n';
  for (int y = 0; y < n; ++y) {
    for (int x = 0; x < n; ++x) {
      m[{y, x}] = (y + x) % 7 == 0 || x % 3 == 0 ? 1 : 0;
    }
  }
  cout << m << '\n';

  AdjList al(n);
  for (int y = 0; y < n; ++y) {
    for (int x = 0; x < n; ++x) {
      if ((y + x) % 7 == 0) al[y].push_back(x);
    }
  }
  cout << al << '\n';

  test_traversal();
}
