#pragma once

#include <vector>
#include "ndpoint.h"

template <typename V>
class IKNearestNeighbor {
 public:
  IKNearestNeighbor() {}
  ~IKNearestNeighbor() {}
  virtual void AddPoint(NDPoint<V> *np) {}
  virtual void Build() {}
  virtual NDPoint<V> Nearest(const NDPoint<V> *np) {
    return NDPoint<V>();
  }
  virtual std::vector<NDPoint<V>> KNearest(const NDPoint<V> *np, int K) {
    return std::vector<NDPoint<V>>(0);
  }
};
