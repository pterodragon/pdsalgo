#ifndef QUICK_SORT_HPP
#define QUICK_SORT_HPP

#include <vector>

#include "algo/quick_select.hpp"

namespace P {
using namespace std;

template <typename E>
void quick_sort(vector<E>& v, int lo, int hi) {
  if (lo >= hi) return;
  auto p = partition_lomuto(v, lo, hi);
  quick_sort(v, lo, p - 1);
  quick_sort(v, p + 1, hi);
}

template <typename E>
void quick_sort(vector<E>& v) {
  quick_sort(v, 0, v.size() - 1);
}

template <typename E>
void quick_sort2(vector<E>& v, int lo, int hi) {
  if (lo >= hi) return;
  auto p = partition_hoare(v, lo, hi);
  quick_sort2(v, lo, p);
  quick_sort2(v, p + 1, hi);
}

template <typename E>
void quick_sort2(vector<E>& v) {
  quick_sort2(v, 0, v.size() - 1);
}

}  // namespace P

#endif /* QUICK_SORT_HPP */
