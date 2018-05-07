#include <cstdio>
#include <cstdlib>
#include <vector>

const int maxn = 100;
const int maxd = 10;
const int maxv = (int)1e2;
const int maxq = 10;
int n, D, q;

inline int RandInt(int bound) {
  return rand() % bound + 1;
}

int main() {
  srand(114514 + 3);
  n = RandInt(maxn);
  D = RandInt(maxd);
  printf("%d %d\n", n, D);

  std::vector<int> vec(D);
  for (int i = 0; i < n; i++) {
    //std::generate(vec.begin(), vec.end(), []{ return RandInt(maxv); });
    for (auto &v : vec)
      v = RandInt(maxv);
    for (auto v : vec)
      printf("%d ", v);
    printf("\n");
  }

  q = RandInt(maxq);
  printf("%d\n", q);
  for (int i = 0; i < q; i++) {
    int k = RandInt(n);
    //std::generate(vec.begin(), vec.end(), []{ return RandInt(maxv); });
    for (auto &v : vec)
      v = RandInt(maxv);
    printf("%d", k);
    for (auto v : vec)
      printf(" %d", v);
    printf("\n");
  }
  return 0;
}
