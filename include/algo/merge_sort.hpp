#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP

#include <vector>

namespace P {
using namespace std;

/*
 * Merge two sorted ranges [p, q] and [q + 1, r]
 */
template <typename E>
void merge(vector<E>& v, int p, int q, int r) {
  auto len = r - p + 1;
  vector<E> temp(len);
  for (int e = 0, a = p, b = q + 1; e < len; ++e) {
    if (a > q)
      temp[e] = v[b++];
    else if (b > r)
      temp[e] = v[a++];
    else if (v[a] <= v[b])
      temp[e] = v[a++];
    else
      temp[e] = v[b++];
  }
  copy(begin(temp), end(temp), begin(v) + p);
}

template <typename E>
void merge_sort(vector<E>& v, int lo, int hi) {
  if (lo >= hi) return;
  int mid = (lo + hi) / 2;
  merge_sort(v, lo, mid);
  merge_sort(v, mid + 1, hi);
  merge(v, lo, mid, hi);
}

template <typename E>
void merge_sort(vector<E>& v) {
  merge_sort(v, 0, (int)v.size() - 1);
}

}  // namespace P
#endif /* MERGE_SORT_HPP */
