#ifndef QUICK_SELECT_HPP
#define QUICK_SELECT_HPP

#include <algorithm>
#include <vector>

namespace P {
using namespace std;

/*
 * Lomuto partition scheme
 *
 * always use v[hi] for the pivot
 *
 * places all elements in the range of [lo, hi] which are smaller than v[hi]
 * to the left in the range, and others are placed to the right in the range
 *
 * Partition is semi-stable: the relative order of the elements which v[hi] >=
 * pivot is preserved
 *
 * return position of the pivot
 */
template <typename E = int>
int partition_lomuto(vector<E> &v, int lo, int hi) {
  auto pivot = v[hi];
  for (int q = lo; q < hi; ++q)
    if (v[q] < pivot) swap(v[q], v[lo++]);
  swap(v[hi], v[lo]);
  return lo;
}

template <typename E = int>
int partition_lomuto(vector<E> &v) {
  return partition_lomuto(v, 0, (int)v.size() - 1);
}

/*
 * Hoare partition scheme
 *
 * Partition is unstable
 *
 * return x s.t.
 * v[:x+1] <= pivot and v[x+1:] >= pivot
 * CAUTION: elements that are equal to pivot can appear in both partition
 *
 * The pivot's final location is not necessarily at the index that was returned:
 * this just guarantees elements of index on or before the pivot index returned
 * <= the pivot chosen originally (v[lo] this case)
 *
 * Important: Pivot uses v[lo] so that returned value < hi
 */
template <typename E = int>
int partition_hoare(vector<E> &v, int lo, int hi) {
  auto pivot = v[lo];
  --lo;
  ++hi;
  while (true) {
    while (v[++lo] < pivot)
      ;
    while (v[--hi] > pivot)
      ;
    if (lo < hi)
      swap(v[lo], v[hi]);
    else
      return hi;
  }
}
template <typename E = int>
int partition_hoare(vector<E> &v) {
  return partition_hoare(v, 0, (int)v.size() - 1);
}

/*
 * Select the kth smallest element from v
 * k is 0-indexed
 */
template <typename E = int>
E quick_select(vector<E> &v, int k, int lo, int hi) {
  if (lo == hi) return v[lo];
  auto p = partition_lomuto(v, lo, hi);
  if (p - lo == k) return v[p];
  if (k < p - lo)
    return quick_select(v, k, lo, p - 1);
  else
    return quick_select(v, k - (p - lo) - 1, p + 1, hi);
}

template <typename E = int>
E quick_select(vector<E> &v, int k) {
  return quick_select(v, k, 0, (int)v.size() - 1);
}

template <typename E = int>
E quick_select2(vector<E> &v, int k, int lo, int hi) {
  if (lo == hi) return v[lo];
  auto p = partition_hoare(v, lo, hi);
  if (p - lo >= k)
    return quick_select2(v, k, lo, p);
  else
    return quick_select2(v, k - (p - lo) - 1, p + 1, hi);
}

template <typename E = int>
E quick_select2(vector<E> &v, int k) {
  return quick_select2(v, k, 0, (int)v.size() - 1);
}

}  // namespace P
#endif /* QUICK_SELECT_HPP */
