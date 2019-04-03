#include <iostream>
#include <string>

#include "ds/graph.hpp"

using namespace P;
using namespace std;

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
}
