#include <catch2/catch.hpp>

#include "algo/merge_sort.hpp"

#include <algorithm>

using namespace std;
using namespace P;

TEST_CASE("merge", "[merge_sort]") {
  SECTION("random") {
    mt19937 gen_;
    uniform_int_distribution dis(0, 10);
    vector<int> v;
    int n =
        GENERATE(1, 2, 3, 4, 15, 16, 17, 18, 19, 20, 1 << 8, (1 << 8) + 1);
    gen_.seed(n);
    uniform_int_distribution dis2(0, n - 1);
    auto gen = [&dis, &gen_]() { return dis(gen_); };
    auto gen2 = [&dis2, &gen_]() { return dis2(gen_); };
    generate_n(back_inserter(v), n, gen);
    int q = gen2();
    CAPTURE(q);
    DYNAMIC_SECTION(n << " numbers") {
      sort(begin(v), begin(v) + q + 1);
      sort(begin(v) + q + 1, end(v));
      // REQUIRE(0 <= q);
      // REQUIRE(q < n - 1);
      merge(v, 0, q, n - 1);
      REQUIRE(is_sorted(begin(v), end(v)));
    }
  }
}

TEST_CASE("merge_sort", "[merge_sort]") {
  SECTION("random") {
    mt19937 gen_;
    uniform_int_distribution dis(0, 10);
    vector<int> v;
    int n =
        GENERATE(0, 1, 2, 3, 4, 15, 16, 17, 18, 19, 20, 1 << 8, (1 << 8) + 1);
    gen_.seed(n);
    uniform_int_distribution dis2(0, n - 1);
    auto gen = [&dis, &gen_]() { return dis(gen_); };
    generate_n(back_inserter(v), n, gen);
    DYNAMIC_SECTION(n << " numbers") {
      merge_sort(v);
      REQUIRE(is_sorted(begin(v), end(v)));
    }
  }
}
