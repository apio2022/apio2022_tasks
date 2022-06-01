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

const int maxn = 400000 + 100;

int n, k;
vector<int> A[maxn], B[maxn];
set<int> L[maxn], R[maxn];
int mark[maxn], cmark = 1234;

void make(int xl, int xr) {
	if(xr - xl == 1) return;
	int xm = (xl + xr) / 2;
	for(int i = xm; i > xl; i--) L[xm].insert(i);
	for(int i = xm; i < xr; i++) R[xm].insert(i);
	make(xl, xm);
	make(xm, xr);
}

void dfs(int u, vector<int> f[], set<int> &s, int newdfs = 1) {
	if(newdfs) cmark++;
	if(mark[u] == cmark) return;
	mark[u] = cmark;
	s.insert(u);
	for(int v: f[u]) dfs(v, f, s, 0);
}

bool go(int xl, int xr, int u, int v) {
	int xm = (xl + xr) / 2;

	if(!in(L[xm], v) && !in(R[xm], u)) return false;
	if(in(L[xm], v) && in(R[xm], u)) return true;
	cerr << " !!!! " << endl;
	if(in(L[xm], v)) {
		if(!in(L[xm], u)) dfs(u, B, L[xm]);
		return go(xl, xm, u, v);
	}
	if(in(R[xm], u)) {
		if(!in(R[xm], v)) dfs(v, A, R[xm]);
		return go(xm, xr, u, v);
	}
	return false;
}

void init(int _n, int _k) {
	n = _n, k = _k;
	make(-1, k);
}

bool add_portal(int u, int v) {
	bool res = go(-1, k, u, v);
	A[u].pb(v), B[v].pb(u);
	return res;
}


int add_teleporter(int u,int v){
	return add_portal(u,v);
}
