#include <cstdlib>
#include <cstring>
#include <vector>
#include <cstdio>
#include <queue>
using namespace std;

const int MAXN = 1000000;
const int MAXK = MAXN;
const int LIM = 10000000;

#define MP make_pair
#define PB push_back 
#define FR(i, a, b) for(int i=(a); i<(b); i++) 
#define FOR(i, n) FR(i, 0, n) 
#define RF(i, a, b) for(int i=(b)-1; i>=(a); i--) 
#define ROF(i, n) RF(i, 0, n) 
#define EACH(it,X) for(__typeof((X).begin()) it=(X).begin(); it!=(X).end(); ++it) 
#define A first
#define B second

int _counter;

int prob, dir;

vector<int> e_from[MAXN];
vector<int> e_to[MAXN];

int max_from[MAXN];
int min_to[MAXN];

int q[MAXN], tail;

int vis[MAXN], global_timer;

int _n, _k;

void ShowValues() {
  for(int i = 0; i < _n; ++i) {
    printf("%d %d", max_from[i], min_to[i]);
  }
  printf("\n");
}

void init(int n, int k) {
  prob = 1;
  dir = 0;

  _counter = 0;
  _n = n;
  _k = k;
  for(int i = 0; i < n; ++i) {
    if(i < k) {
      max_from[i] = i;
      min_to[i] = i;
    } else {
      max_from[i] = -1;
      min_to[i] = MAXN;
    }
    vis[i] = 0;
  }
  global_timer = 0;
  _counter = 0;
}

inline bool UpdateMax(int u, int val) {
  if(u < _k) {
    if(val >= u) {
      return true;
    } else {
      return false;
    }
  }
  if(val > max_from[u] & vis[u] != global_timer && rand() % prob == 0) {
    vis[u] = global_timer;
    max_from[u] = val;
    q[tail++] = u;
  }
  return false;
}

inline bool UpdateMin(int u, int val) {
  if(u < _k) {
    if(val <= u) {
      return true;
    } else {
      return false;
    }
  }
  if(val < min_to[u] & vis[u] != global_timer && rand() % prob == 0) {
    vis[u] = global_timer;
    min_to[u] = val;
    q[tail++] = u;
  }
  return false;
}

priority_queue<pair<int, int> > Q;

void ConsiderMin(int u, int val) {
  if(val < min_to[u]) {
    min_to[u] = val;
    Q.push(MP(-min_to[u], u));
  }
}

void BuildMin() {
  while(!Q.empty()) {
    Q.pop();
  }
  FOR(i, _n) {
    min_to[i] = MAXN;
  }
  FOR(i, _k) {
    ConsiderMin(i, i);
  }
  while(!Q.empty()) {
    int u = Q.top().B;
    Q.pop();
    EACH(v, e_from[u]) {
      ConsiderMin(*v, min_to[u]);
    }
  }
}

void ConsiderMax(int u, int val) {
  if(val > max_from[u]) {
    max_from[u] = val;
    Q.push(MP(max_from[u], u));
  }
}

void BuildMax() {
  while(!Q.empty()) {
    Q.pop();
  }
  FOR(i, _n) {
    max_from[i] = -1;
  }
  FOR(i, _k) {
    ConsiderMax(i, i);
  }
  while(!Q.empty()) {
    int u = Q.top().B;
    Q.pop();
    EACH(v, e_to[u]) {
      ConsiderMax(*v, max_from[u]);
    }
  }
}


int add_teleporter(int u, int v) {
  e_to[u].push_back(v);
  e_from[v].push_back(u);

  tail = 0;
  global_timer++;
  if(dir == 0) {
    if(UpdateMax(v, max_from[u])) {
      return true;
    }
    for(int i = 0; i < tail; ++i) {
      int p = q[i];
      for(int j = 0; j < e_to[p].size(); ++j) {
        if(UpdateMax(e_to[p][j], max_from[p])) {
          return true;
        }
      }
    }
  } else {
    if(UpdateMin(u, min_to[v])) {
      return true;
    }
    for(int i = 0; i < tail; ++i) {
      int p = q[i];
      for(int j = 0; j < e_from[p].size(); ++j) {
        if(UpdateMin(e_from[p][j], min_to[p])) {
          return true;
        }
      }
    }
  }

  _counter += tail;
  if(_counter > LIM) {
    _counter = 0;
    if(dir == 0) {
      dir = 1;
      BuildMin();
    } else {
      dir = 0;
      prob *= 2;
      BuildMax();
    }
//fprintf(stderr, "SWITCHING to DIR = %d, PROB = %d\n", dir, prob);
  }

  return false;
}
