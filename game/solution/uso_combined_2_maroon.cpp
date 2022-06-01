#include "game.h"

#include <bits/stdc++.h>
using namespace std;

namespace yosupo{
template<class T> using V = vector<T>;
template<class T> using VV = V<V<T>>;

static VV<int> g, rg;
static V<int> low, up;
static int K;

struct UnionFind {
    V<int> ig;
    VV<int> gi;
    int gc;  // group count
    V<int> lv, uv;
    UnionFind() {}
    UnionFind(int n, int low, int up) {
        ig = V<int>(n);
        gi = VV<int>(n);
        for (int i = 0; i < n; i++) {
            ig[i] = i; gi[i] = {i};
        }
        gc = n;
        lv = V<int>(n, low);
        uv = V<int>(n, up);
    }
    bool merge(int a, int b) {
        if (same(a, b)) return false;
        gc--;
        int x = ig[a], y = ig[b];
        if (gi[x].size() < gi[y].size()) swap(x, y);
        for (int j: gi[y]) {
            ig[j] = x;
        }
        gi[x].insert(gi[x].end(), gi[y].begin(), gi[y].end());
        gi[y].clear();
        lv[x] = max(lv[x], lv[y]);
        uv[x] = min(uv[x], uv[y]);
        return uv[x] <= lv[x];
    }
    bool same(int a, int b) { return ig[a] == ig[b]; }
};
static UnionFind uf;

void init(int N, int _K) {
    K = _K;
    g = VV<int>(N);
    rg = VV<int>(N);
    uf = UnionFind(N-K, 0, K+1);
/*    low = V<int>(N, 0);
    up = V<int>(N, K+1);
    for (int i = 0; i < K; i++) {
        low[i] = up[i] = i+1;
    }*/

}


/*
bool dfs(int p, int l, int r) {
    if (p < K) return false;
    bool f = false;
    if (low[p] < l) {
        low[p] = l;
        f = true;
    }
    if (r < up[p]) {
        up[p] = r;
        f = true;
    }
    if (up[p] <= low[p]) return true;
    if (!f) return false;
    for (int d: g[p]) if (dfs(d, low[p], up[p])) return true;
    for (int d: rg[p]) if (dfs(d, low[p], up[p])) return true;
    return false;
}*/

int add_teleporter(int u, int v) {
    if (max(u, v) < K) {
        return u >= v;
    }
    g[u].push_back(v);
    rg[v].push_back(u);
    if (u < K) {
        int g = uf.ig[v - K];
        uf.lv[g] = max(uf.lv[g], u+1);
        return uf.uv[g] <= uf.lv[g];
    }
    if (v < K) {
        int g = uf.ig[u - K];
        uf.uv[g] = min(uf.uv[g], v+1);
        return uf.uv[g] <= uf.lv[g];
    }
    return uf.merge(u - K, v - K);
//    return (dfs(v, low[u], K+1) || dfs(u, 0, up[v]));
}
}

namespace iran{
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


bool add_portal(int u, int v) {
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


int add_teleporter(int u,int v){
	return add_portal(u,v);
}

}

int mode=0;

int n,k;

void init(int N,int K){
	n=N;
	k=K;
	yosupo::init(n,k);
}

vector<pair<int,int>> added;

int add_teleporter(int u,int v){
	if(mode==0){
		if(yosupo::add_teleporter(u,v)){
			mode=1;
			iran::init(n,k);
			for(auto e:added){
				iran::e_to[e.first].push_back(e.second);
				iran::e_from[e.second].push_back(e.first);
			}
			iran::BuildMax();
			iran::BuildMin();
		}
	}
	if(mode==1){
		if(iran::add_teleporter(u,v))
			return 1;
	}
	added.push_back(make_pair(u,v));
	return 0;
}
