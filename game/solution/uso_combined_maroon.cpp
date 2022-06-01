#include "game.h"

#include <vector>
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

namespace richard{
#define rep(i, n) for (int i = 0, _n = (int)(n); i < _n; i++)
#define fer(i, x, n) for (int i = (int)(x), _n = (int)(n); i < _n; i++)
#define rof(i, n, x) for (int i = (int)(n), _x = (int)(x); i-- > _x; )
#define sz(x) (int((x).size()))
#define pb push_back
#define all(X) (X).begin(),(X).end()
#define X first
#define Y second
#define in(X, a) ((X).find(a) != (X).end())

template<class P, class Q> inline void smin(P &a, Q b) { if (b < a) a = b; }
template<class P, class Q> inline void smax(P &a, Q b) { if (a < b) a = b; }

typedef long long ll;
typedef pair<int, int> pii;

////////////////////////////////////////////////////////////////////////////////

const int inf = 1 << 22;
const int maxn = 500000 + 100;

const double UPD_RATIO = 0.8;
const int CTV = 4000;

int n, k;
vector<int> out[maxn], in[maxn];
int a[maxn], b[maxn], c[maxn];

bool dfs(int u, int mn, int mx) {
	if(u < k) return false;
	smax(a[u], mn), smin(b[u], mx);
	if(b[u] <= a[u]) return true;
	if(b[u] - a[u] <= UPD_RATIO * c[u]) {
		c[u] = b[u] - a[u];
		for(int v: out[u]) if(dfs(v, a[u], inf)) return true;
		for(int v:  in[u]) if(dfs(v, -1,  b[u])) return true;
	}
	return false;
}

void init(int _n, int _k) {
	n = _n, k = _k;
	rep(i, k) a[i] = b[i] = i + 1, c[i] = 0;
	fer(i, k, n) a[i] = 0, b[i] = k + 1, c[i] = k + 1;
}

bool update(int u, int mn, int mx) {
	if(u < k) return false;
	smax(a[u], mn), smin(b[u], mx);
	if(b[u] <= a[u]) return true;
	if(b[u] - a[u] < c[u]) {
		c[u] = b[u] - a[u];
		for(int v: out[u]) if(update(v, a[u], inf)) return true;
		for(int v:  in[u]) if(update(v, -1,  b[u])) return true;
	}
	return false;
}

int cnt = 0;
int add_teleporter(int u, int v) { // 0-based
	if(++cnt >= CTV) {
		rep(u, k) for(int v: in[u]) if(update(v, -1, b[u])) return true;
		rof(u, k, 0) for(int v: out[u]) if(update(v, a[u], inf)) return true;
		cnt = 0;
	}

	if(v <= u && u < k) return true; 
	out[u].pb(v), in[v].pb(u);
	return dfs(v, a[u], inf) || dfs(u, -1,  b[v]);
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
			richard::init(n,k);
			for(auto e:added){
				richard::out[e.first].push_back(e.second);
				richard::in[e.second].push_back(e.first);
			}
			richard::cnt=richard::CTV;
		}
	}
	if(mode==1){
		if(richard::add_teleporter(u,v))
			return 1;
	}
	added.push_back(make_pair(u,v));
	return 0;
}
