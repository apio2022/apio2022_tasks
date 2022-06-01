#include "game.h"

#include <vector>
using namespace std;
template<class T> using V = vector<T>;
template<class T> using VV = V<V<T>>;

static VV<int> g, rg;
static V<int> low, up;
static int K;

void init(int N, int _K) {
    K = _K;
    g = VV<int>(N);
    rg = VV<int>(N);
    low = V<int>(N, 0);
    up = V<int>(N, K+1);
    for (int i = 0; i < K; i++) {
        low[i] = up[i] = i+1;
    }
}

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
}

int add_teleporter(int u, int v) {
    if (max(u, v) < K) {
        return u >= v;
    }
    g[u].push_back(v);
    rg[v].push_back(u);
    return (dfs(v, low[u], K+1) || dfs(u, 0, up[v]));
}
