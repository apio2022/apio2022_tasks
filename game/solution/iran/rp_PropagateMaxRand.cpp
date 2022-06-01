#include <cstdlib>
#include <cstring>
#include <vector>
#include <cstdio>
#include <ctime>
using namespace std;

const int MAXN = 1000000;
const int MAXK = MAXN;

vector<int> e[MAXN];

int max_from[MAXN];

int q[MAXN], tail;

int vis[MAXN], global_timer;

int _n, _k;

int _counter;

void ShowValues() {
  for(int i = 0; i < _n; ++i) {
    printf("%d ", max_from[i]);
  }
  printf("\n");
}

void init(int n, int k) {
  srand(time(0));
  _counter = 0;
  _n = n;
  _k = k;
  for(int i = 0; i < n; ++i) {
    if(i < k) {
      max_from[i] = i;
    } else {
      max_from[i] = -1;
    }
    vis[i] = 0;
  }
  global_timer = 0;
}

inline bool update(int u, int val) {
  if(u < _k) {
    if(val >= u) {
      return true;
    } else {
      return false;
    }
  }
if(rand() % 2 == 0) return false;
  if(val > max_from[u] & vis[u] != global_timer) {
//printf("%d --> %d \n", u, val); fflush(stdout);
   vis[u] = global_timer;
    max_from[u] = val;
    q[tail++] = u;
  }
  return false;
}

bool add_portal(int u, int v) {
//_counter++;
//if(_counter % 1000 == 0) fprintf(stderr, "%d\n", _counter);
//fprintf(stderr, "ADDING %d %d\n", u, v); fflush(stdout);
  e[u].push_back(v);

  tail = 0;
  global_timer++;
  if(update(v, max_from[u])) {
    return true;
  }
  for(int i = 0; i < tail; ++i) {
    int p = q[i];
    for(int j = 0; j < e[p].size(); ++j) {
      if(update(e[p][j], max_from[p])) {
        return true;
      }
    }
  }
  return false;
}

int add_teleporter(int u,int v){
	return add_portal(u,v);
}
