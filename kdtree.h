#pragma once

#include <cstdio>
#include <memory>
#include <queue>
#include <sstream>
#include <utility>
#include <vector>
#include "knn.h"
#include "ndpoint.h"

template <typename V>
class KdTree final : public IKNearestNeighbor<V> {
 public:
  struct Node {
    Node() {}
    ~Node() {}
    std::shared_ptr<NDPoint<V>> p;
    struct Node *lc, *rc;
    int dep;
    /* boundary */
    V lef, rig;
  };

  KdTree() {}
  ~KdTree() { ReleaseNodes(root_); }

  explicit KdTree(int Dim) { Dim_ = Dim; }

  void AddPoint(NDPoint<V> *np) override { points_.push_back(std::shared_ptr<NDPoint<V>>(np)); }

  void Build() override { root_ = BuildRecursive(0, points_.size(), 0); }

  /* TODO(cjr) return rvalue to avoid the copy */
  NDPoint<V> Nearest(const NDPoint<V> *np) override {
    double min_dist = 1e30;
    NDPoint<V> *ans = nullptr;
    _Nearest(root_, np, min_dist, &ans);
    return *ans;
  }

  /* TODO(cjr) return rvalue to avoid the copy */
  std::vector<NDPoint<V>> KNearest(const NDPoint<V> *np, int K) override {
    if (K > static_cast<int>(points_.size())) K = static_cast<int>(points_.size());
    std::vector<NDPoint<V>> rets;
    auto cmp = [np](const NDPoint<V> *p1, const NDPoint<V> *p2) {
      return np->distance(p1) < np->distance(p2);
    };
    std::priority_queue<NDPoint<V> *, std::vector<NDPoint<V> *>, decltype(cmp)> pq(cmp);
    _KNearest(root_, np, K, pq);
    while (!pq.empty()) {
      rets.push_back(*pq.top());
      pq.pop();
    }
    std::reverse(rets.begin(), rets.end());
    return rets;
  }

 private:
  void ReleaseNodes(Node *n) {
    if (n) {
      ReleaseNodes(n->lc);
      ReleaseNodes(n->rc);
      delete n;
    }
  }

  /* TODO(cjr) can be replace by std::nth_element */
  std::shared_ptr<NDPoint<V>> QuickSelect(int l, int r, int rank, int d) {
    int i = l, j = r;
    V mid = points_[(l + r) >> 1]->val[d];
    do {
      while (points_[i]->val[d] < mid) i++;
      while (mid < points_[j - 1]->val[d]) j--;
      if (i < j) {
        std::swap(points_[i], points_[j - 1]);
        ++i;
        --j;
      }
    } while (i < j);
    if (l < j && rank < j - l) return QuickSelect(l, j, rank, d);
    if (i < r && rank >= i - l) return QuickSelect(i, r, rank - (i - l), d);
    return points_[(j + i) >> 1];
  }

  std::pair<V, V> GetMinMax(int l, int r, int d) {
    auto res = std::minmax_element(
        points_.begin() + l, points_.begin() + r,
        [d](const std::shared_ptr<NDPoint<V>> &p1, const std::shared_ptr<NDPoint<V>> &p2) {
          return (p1.get())->val[d] < (p2.get())->val[d];
        });
    return std::make_pair((res.first->get())->val[d], (res.second->get())->val[d]);
  }

  Node *BuildRecursive(int l, int r, int dep) {
    int mid = (l + r) >> 1;
    int d = dep % Dim_;
    if (l >= r) return nullptr;
    Node *n = new Node();
    std::pair<V, V> min_max = GetMinMax(l, r, d);
    if (dep == 0) {
      n->lef = min_max.first;
      n->rig = min_max.second;
    }
    n->p = QuickSelect(l, r, (r - l) >> 1, d);
    n->dep = dep;
    n->lc = BuildRecursive(l, mid, dep + 1);
    if (n->lc) {
      n->lc->lef = min_max.first;
      n->lc->rig = n->p->val[d];
      // printf("%d %d %d\n", n->lc->p->val[0], n->lc->lef, n->lc->rig);
    }
    n->rc = BuildRecursive(mid + 1, r, dep + 1);
    if (n->rc) {
      n->rc->lef = n->p->val[d];
      n->rc->rig = min_max.second;
      // printf("%d %d %d\n", n->rc->p->val[0], n->rc->lef, n->rc->rig);
    }
    return n;
  }

  void _Nearest(Node *n, const NDPoint<V> *target, double &cur_dist, NDPoint<V> **cur_p) {
    if (n == nullptr) return;
    if (*cur_p == nullptr) {
      *cur_p = n->p.get();
      cur_dist = target->distance(*cur_p);
      _Nearest(n->lc, target, cur_dist, cur_p);
      _Nearest(n->rc, target, cur_dist, cur_p);
      return;
    }
    /* TODO(cjr) remote this % after profiling */
    int d = (n->dep - 1) % Dim_;
    double tmp = 0;
    if (target->val[d] < n->lef) tmp = n->lef - target->val[d];
    if (target->val[d] > n->rig) tmp = target->val[d] - n->rig;
    if (tmp > cur_dist) return;
    tmp = target->distance(n->p.get());
    if (tmp < cur_dist) {
      cur_dist = tmp;
      *cur_p = n->p.get();
    }
    _Nearest(n->lc, target, cur_dist, cur_p);
    _Nearest(n->rc, target, cur_dist, cur_p);
  }

  void _KNearest(Node *n, const NDPoint<V> *target, int K, auto &pq) {
    if (n == nullptr) return;
    if (pq.size() < static_cast<size_t>(K)) {
      pq.push(n->p.get());
      _KNearest(n->lc, target, K, pq);
      _KNearest(n->rc, target, K, pq);
      return;
    }
    int d = (n->dep - 1) % Dim_;
    double tmp = 0;
    if (target->val[d] < n->lef) tmp = n->lef - target->val[d];
    if (target->val[d] > n->rig) tmp = target->val[d] - n->rig;
    double cur_dist = target->distance(pq.top());
    if (tmp > cur_dist) return;
    tmp = target->distance(n->p.get());
    if (tmp < cur_dist) {
      pq.pop();
      pq.push(n->p.get());
    }
    _KNearest(n->lc, target, K, pq);
    _KNearest(n->rc, target, K, pq);
  }

 private:
  int Dim_;
  struct Node *root_;
  std::vector<std::shared_ptr<NDPoint<V>>> points_;
};
