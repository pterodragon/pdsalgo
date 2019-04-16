#include <catch2/catch.hpp>

#include "algo/quick_select.hpp"

#include <algorithm>
#include <climits>

using namespace std;
using namespace P;

TEST_CASE("partition", "[quick_select]") {
  SECTION("random") {
    mt19937 gen_;
    uniform_int_distribution dis(0, 10);
    vector<int> v;
    int n = GENERATE(1, 2, 3, 4, 15, 16, 17, 18, 19, 20, 1 << 8);
    gen_.seed(n);
    auto gen = [&dis, &gen_]() { return dis(gen_); };
    generate_n(back_inserter(v), n, gen);
    DYNAMIC_SECTION(n << " elements (loumuto)") {
      auto pivot_condition = [pivot = v.back()](auto e) { return e < pivot; };
      auto pivot_pos = partition_lomuto(v);
      auto it_pos = distance(
          begin(v), partition_point(v.begin(), v.end(), pivot_condition));
      CAPTURE(v);
      REQUIRE(pivot_pos == it_pos);
      REQUIRE(is_partitioned(begin(v), end(v), pivot_condition));
    }
    DYNAMIC_SECTION(n << " elements (hoare)") {
      CAPTURE(v);
      auto pivot = v.front();
      auto pivot_pos = partition_hoare(v);
      CAPTURE(pivot_pos);
      CAPTURE(v);
      REQUIRE(all_of(begin(v), begin(v) + pivot_pos,
                     [pivot](auto e) { return e <= pivot; }));
      REQUIRE(all_of(begin(v) + pivot_pos + 1, end(v),
                     [pivot](auto e) { return e >= pivot; }));
    }
  }
  SECTION("same numbers") {
    int n = GENERATE(1, 2, 3, 4, 15, 16, 17, 18, 19, 20, 1 << 8);
    vector<int> v(n);  // all 0's
    SECTION("loumuto") {
      auto pivot_condition = [pivot = v.back()](auto e) { return e < pivot; };
      auto pivot_pos = partition_lomuto(v);
      auto it_pos = distance(
          begin(v), partition_point(v.begin(), v.end(), pivot_condition));
      CAPTURE(v);
      REQUIRE(pivot_pos == it_pos);
      REQUIRE(is_partitioned(begin(v), end(v), pivot_condition));
    }
    SECTION("hoare") {
      CAPTURE(v);
      auto pivot = v.front();
      auto pivot_pos = partition_hoare(v);
      CAPTURE(pivot_pos);
      CAPTURE(v);
      REQUIRE(all_of(begin(v), begin(v) + pivot_pos,
                     [pivot](auto e) { return e <= pivot; }));
      REQUIRE(all_of(begin(v) + pivot_pos + 1, end(v),
                     [pivot](auto e) { return e >= pivot; }));
    }
  }
}

TEST_CASE("quick_select", "[quick_select]") {
  mt19937 gen;
  vector<int> v;
  int n = GENERATE(1, 2, 3, 4, 15, 16, 17, 18, 19, 20, 1 << 8);
  generate_n(back_inserter(v), n, gen);
  gen.seed(n);
  uniform_int_distribution dis(0, n - 1);
  DYNAMIC_SECTION(n << " elements (1)") {
    int times = 10;
    while (--times > 0) {
      CAPTURE(v);
      int k = dis(gen);
      CAPTURE(k);
      int res = quick_select(v, k);
      nth_element(begin(v), begin(v) + k, end(v));
      REQUIRE(res == v[k]);
      shuffle(begin(v), end(v), gen);
    }
  }
  DYNAMIC_SECTION(n << " elements (2)") {
    int times = 10;
    while (--times > 0) {
      CAPTURE(v);
      int k = dis(gen);
      CAPTURE(k);
      int res = quick_select2(v, k);
      CAPTURE(v);
      nth_element(begin(v), begin(v) + k, end(v));
      REQUIRE(res == v[k]);
      shuffle(begin(v), end(v), gen);
    }
  }
}

TEST_CASE("quick_select e1", "[quick_select]") {
  vector<int> v{-795755684, 581869302};
  int k = 0;
  int res = quick_select2(v, k);
  CAPTURE(v);
  nth_element(begin(v), begin(v) + k, end(v));
  REQUIRE(res == v[k]);
}
