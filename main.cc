#include <cassert>
#include <iostream>
#include "bruteforce.h"
#include "kdtree.h"
#include "knn.h"
#include "ndpoint.h"

typedef double Val;

const double eps = 1e-8;

const int maxn = 1000;
int n, D, q;
Val vec[maxn];

void PrintResult(auto rets) {
  if (rets.size() > 100) return;
  std::cout << "[" << rets[0];
  for (size_t i = 1; i < rets.size(); i++) std::cout << "," << rets[i];
  std::cout << "]" << std::endl;
}

void CheckResult(NDPoint<Val> *np, int K, auto rets, auto rets2) {
  assert(static_cast<size_t>(K) == rets.size());
  assert(static_cast<size_t>(K) == rets2.size());
  for (int i = 0; i < K; i++) {
    //printf("%.6f %.6f\n", np->distance(rets[i]), np->distance(rets2[i]));
    assert(std::abs(np->distance(rets[i]) - np->distance(rets2[i])) < eps);
  }
}

int main() {
  std::ios::sync_with_stdio(false);

  std::cin >> n >> D;
  IKNearestNeighbor<Val> *knn = new KdTree<Val>(D);
  IKNearestNeighbor<Val> *knn2 = new BruteForce<Val>();

  for (int i = 0; i < n; i++) {
    for (int i = 0; i < D; i++) {
      std::cin >> vec[i];
    }
    NDPoint<Val> *np = new NDPoint<Val>(vec, D);
    knn->AddPoint(np);
    NDPoint<Val> *np2 = new NDPoint<Val>(*np);
    knn2->AddPoint(np2);
  }

  knn->Build();
  knn2->Build();

  std::cin >> q;
  for (int i = 0; i < q; i++) {
    int k;
    std::cin >> k;
    for (int i = 0; i < D; i++) {
      std::cin >> vec[i];
    }
    NDPoint<Val> *np = new NDPoint<Val>(vec, D);
    NDPoint<Val> ret = knn->Nearest(np);
    NDPoint<Val> ret2 = knn2->Nearest(np);
    std::cout << ret << " " << ret2 << std::endl;

    auto rets = knn->KNearest(np, k);
    auto rets2 = knn2->KNearest(np, k);
    PrintResult(rets);
    PrintResult(rets2);

    CheckResult(np, k, rets, rets2);
  }

  std::cout << "\033[1;32mPass test\033[0m" << std::endl;
  return 0;
}
