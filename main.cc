#include <iostream>
#include "kdtree.h"
#include "knn.h"
#include "ndpoint.h"

const int maxn = 1000;
int n, D, q;
int vec[maxn];

int main() {
  std::ios::sync_with_stdio(false);

  std::cin >> n >> D;
  IKNearestNeighbor<int> *knn = new KdTree<int>(D);

  for (int i = 0; i < n; i++) {
    for (int i = 0; i < D; i++) {
      std::cin >> vec[i];
    }
    NDPoint<int> *np = new NDPoint<int>(vec, D);
    knn->AddPoint(np);
  }

  knn->Build();

  std::cin >> q;
  for (int i = 0; i < q; i++) {
    int k;
    std::cin >> k;
    for (int i = 0; i < D; i++) {
      std::cin >> vec[i];
    }
    NDPoint<int> *np = new NDPoint<int>(vec, D);
    NDPoint<int> ret = knn->Nearest(np);
    std::cout << ret << std::endl;
  }
  return 0;
}
