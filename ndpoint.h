#pragma once

#include <valarray>
#include <cmath>
#include <iostream>

template <typename V>
struct NDPoint {
  NDPoint() {}
  ~NDPoint() {}
  /**
   * \breif construct from a c-array
   */
  NDPoint(V *data, size_t size) {
    val = std::valarray<V>(data, size);
  }
  /**
   * \breif construct from another NDPoint<V>
   */
  NDPoint(const NDPoint<V> &p) {
    val = p.val;
  }
  double distance(const NDPoint<V> &p) const {
    return std::sqrt(std::pow((val - p.val), 2).sum());
  }
  double distance(const NDPoint<V> *p) const {
    return std::sqrt(std::pow((val - p->val), 2).sum());
  }
  std::valarray<V> val;
};

template <typename V>
std::ostream &operator<<(std::ostream &os, const NDPoint<V> &p) {
  os << "[" << p.val[0];
  for (size_t i = 1; i < p.val.size(); i++)
    os << "," << p.val[i];
  os << "]";
  return os;
}
