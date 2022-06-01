#include "game.h"

#include <vector>
using namespace std;
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
