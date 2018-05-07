#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>

typedef int Val;

const int maxn = 10000;
const int maxd = 20;
const Val maxv = (Val)1e9;
const int maxq = 1000;
int n, D, q;

inline int RandInt(int bound) {
  return rand() % bound + 1;
}

int main() {
  srand(114514 + 3);
  n = RandInt(maxn);
  D = RandInt(maxd);
  printf("%d %d\n", n, D);

  std::vector<Val> vec(D);
  for (int i = 0; i < n; i++) {
    //std::generate(vec.begin(), vec.end(), []{ return RandInt(maxv); });
    for (auto &v : vec)
      v = RandInt((int)maxv);
    for (auto v : vec)
      std::cout << v << " ";
    std::cout << std::endl;
  }

  q = RandInt(maxq);
  printf("%d\n", q);
  for (int i = 0; i < q; i++) {
    int k = RandInt(n);
    //std::generate(vec.begin(), vec.end(), []{ return RandInt(maxv); });
    for (auto &v : vec)
      v = RandInt((int)maxv);
    printf("%d", k);
    for (auto v : vec)
      std::cout << " " << v;
    std::cout << std::endl;
  }
  return 0;
}
