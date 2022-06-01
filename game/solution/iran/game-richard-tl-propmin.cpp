#include <cstdlib>
#include <cstring>
#include <vector>
#include <cstdio>
using namespace std;

const int MAXN = 1000000;
const int MAXK = MAXN;

vector<int> eFrom[MAXN];

int min_to[MAXN];

int q[MAXN], tail;

int vis[MAXN], global_timer;

int _n, _k;

int _counter;

void ShowValues() {
  for(int i = 0; i < _n; ++i) {
    printf("%d ", min_to[i]);
  }
  printf("\n");
}

void init(int n, int k) {
_counter = 0;
  _n = n;
  _k = k;
  for(int i = 0; i < n; ++i) {
    if(i < k) {
      min_to[i] = i;
    } else {
      min_to[i] = MAXN;
    }
    vis[i] = 0;
  }
  global_timer = 0;
}

inline bool update(int u, int val) {
  if(u < _k) {
    if(val <= u) {
      return true;
    } else {
      return false;
    }
  }
  if(val < min_to[u] & vis[u] != global_timer) {
//printf("%d --> %d \n", u, val); fflush(stdout);
    vis[u] = global_timer;
    min_to[u] = val;
    q[tail++] = u;
  }
  return false;
}

bool add_portal(int u, int v) {
_counter++;
if(_counter % 1000 == 0) fprintf(stderr, "%d\n", _counter);
//fprintf(stderr, "ADDING %d %d\n", u, v); fflush(stdout);
  eFrom[v].push_back(u);

  tail = 0;
  global_timer++;
  if(update(u, min_to[v])) {
    return true;
  }
  for(int i = 0; i < tail; ++i) {
    int p = q[i];
    for(int j = 0; j < eFrom[p].size(); ++j) {
      if(update(eFrom[p][j], min_to[p])) {
        return true;
      }
    }
  }
  return false;
}
