#include <catch2/catch.hpp>

#include "algo/quick_sort.hpp"

#include <algorithm>

using namespace std;
using namespace P;

TEST_CASE("quick_sort", "[quick_sort]") {
  SECTION("random") {
    mt19937 gen_;
    uniform_int_distribution dis(0, 10);
    vector<int> v;
    int n = GENERATE(1, 2, 3, 4, 15, 16, 17, 18, 19, 20, 1 << 8);
    gen_.seed(n);
    auto gen = [&dis, &gen_]() { return dis(gen_); };
    generate_n(back_inserter(v), n, gen);
    DYNAMIC_SECTION(n << " elements (loumuto)") {
      quick_sort(v);
      REQUIRE(is_sorted(begin(v), end(v)));
    }
    DYNAMIC_SECTION(n << " elements (hoare)") {
      quick_sort2(v);
      REQUIRE(is_sorted(begin(v), end(v)));
    }
  }
}
