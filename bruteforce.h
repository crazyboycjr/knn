#pragma once

#include <algorithm>
#include <vector>
#include "knn.h"
#include "ndpoint.h"

template <typename V>
class BruteForce final : public IKNearestNeighbor<V> {
 public:
  BruteForce() {}
  ~BruteForce() {
    for (auto p : points_) delete p;
  }
  void AddPoint(NDPoint<V> *np) override { points_.push_back(np); }
  void Build() override {
    // do nothing
  }
  NDPoint<V> Nearest(const NDPoint<V> *np) override {
    auto res = *std::min_element(points_.begin(), points_.end(),
                                 [np](const NDPoint<V> *p1, const NDPoint<V> *p2) {
                                   return np->distance(p1) < np->distance(p2);
                                 });
    return *res;
  }
  std::vector<NDPoint<V>> KNearest(const NDPoint<V> *np, int K) override {
    if (K > static_cast<int>(points_.size()))
      K = static_cast<int>(points_.size());
    std::vector<NDPoint<V>> rets(K);
    std::partial_sort(points_.begin(), points_.begin() + K, points_.end(),
                      [np](const NDPoint<V> *p1, const NDPoint<V> *p2) {
                        return np->distance(p1) < np->distance(p2);
                      });
    std::transform(points_.begin(), points_.begin() + K, rets.begin(),
        [](const NDPoint<V> *p){ return *p; });
    return rets;
  }

 private:
  std::vector<NDPoint<V> *> points_;
};
