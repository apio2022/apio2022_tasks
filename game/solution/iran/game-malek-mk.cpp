// .... .... .....!
// ...... ......!
// .... ....... ..... ..!
// ...... ... ... .... ... .... .....!
// ... .. ... .... ...?

#include "game.h"
#include<bits/stdc++.h>
using namespace std;

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

const int inf = 1 << 20;
const int maxn = 500000 + 100;

int n, k;
vector<int> out[maxn], in[maxn];
int a[maxn], b[maxn], c[maxn];

bool dfs(int u, int mn, int mx) {
	if(u < k) return false;
	if(mn <= a[u] && b[u] <= mx) return false;
	smax(a[u], mn), smin(b[u], mx);
	if(b[u] <= a[u]) return true;
	for(int v: out[u]) if(dfs(v, a[u], inf)) return true;
	for(int v:  in[u]) if(dfs(v, -1,  b[u])) return true;
	return false;
}

void init(int _n, int _k) {
	n = _n, k = _k;
	int hb; for(hb = k + 1; hb & (hb - 1); hb &= hb - 1);
	rep(i, k) a[i] = b[i] = i + 1, c[i] = 0;
	fer(i, k, n) a[i] = 0, b[i] = k + 1, c[i] = hb;
}

bool add_portal(int u, int v) { // 0-based
	if(v <= u && u < k) return true; 
	out[u].pb(v), in[v].pb(u);
	if(dfs(v, a[u], inf)) return true;
	if(dfs(u, -1,  b[v])) return true;
	return false;
}

int add_teleporter(int u,int v){
	return add_portal(u,v);
}
