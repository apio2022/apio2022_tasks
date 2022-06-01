// .... .... .....!
// ...... ......!
// .... ....... ..... ..!
// ...... ... ... .... ... .... .....!
// ... .. ... .... ...?

#include "game.h"
#include<iostream>
#include<vector>
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

const int inf = 1 << 25;
const int maxn = 300000 + 100;
const int maxm = 500000 + 100;
const int tofctv = 1000;
const double UPD_RATIO = 0.8;

int n, k;
int a[maxn], b[maxn], c[maxn];

int im, om;
int ihead[maxn], ohead[maxn];
int itail[maxn], otail[maxn];
int ito[maxm], oto[maxm];
int iprev[maxm], oprev[maxm];

int id[maxn];

bool dfs(int u, int mn, int mx) {
	if(u < k) return false;
	u = id[u];
//	int t = a[u] ^ b[u];
	smax(a[u], mn), smin(b[u], mx);
	if(b[u] <= a[u]) return true;
//	if(__builtin_clz(t) != __builtin_clz(a[u] ^ b[u])) {
	if(b[u] - a[u] <= UPD_RATIO * c[u]) {
		c[u] = b[u] - a[u];
		for(int e = ohead[u]; e != -1; e = oprev[e]) {
			int v = oto[e];
			if(dfs(v, a[u], inf)) return true;
		}
		for(int e = ihead[u]; e != -1; e = iprev[e]) {
			int v = ito[e];
			if(dfs(v, -1, b[u])) return true;
		}
	}
	return false;
}

int vis[maxn], vmark = 1;
int arr[maxn], apos;

void dfs1(int u) {
	u = id[u]; if(u < k || vis[u] == vmark) return; vis[u] = vmark;
	for(int e = ohead[u]; e != -1; e = oprev[e]) dfs1(oto[e]);
	arr[apos++] = u;
}

void dfs2(int u, int uh) {
	u = id[u]; if(u < k || vis[u] == vmark) return; vis[u] = vmark;

	id[u] = uh;
	smax(a[uh], a[u]); smin(b[uh], b[u]);
	c[uh] = b[uh] - a[uh];

	for(int e = ihead[u]; e != -1; e = iprev[e]) dfs2(ito[e], uh);

	if(itail[uh] != -1) iprev[itail[uh]] = ihead[u];
	else ihead[uh] = ihead[u];
	if(itail[u] != -1) itail[uh] = itail[u];
	ihead[u] = itail[u] = -1;

	if(otail[uh] != -1) oprev[otail[uh]] = ohead[u];
	else ohead[uh] = ohead[u];
	if(otail[u] != -1) otail[uh] = otail[u];
	ohead[u] = otail[u] = -1;
}

void scc() {
	apos = 0;
	vmark++; fer(i, k, n) if(i == id[i]) dfs1(i);
	vmark++;
	rof(i, apos, 0) if(vis[arr[i]] != vmark) {
		int u = arr[i];
		int cnt = 0;
		vis[u] = vmark;
		for(int e = ihead[u]; e != -1; cnt++, e = iprev[e]);
		for(int e = ihead[u]; cnt--; e = iprev[e]) dfs2(ito[e], u);
	}
}

void init(int _n, int _k) {
	n = _n;
	k = _k;

	rep(i, k) a[i] = b[i] = i + 1, c[i] = 0;
	fer(i, k, n) a[i] = 0, b[i] = k + 1, c[i] = k + 1;

	im = om = 0;
	rep(i, n) ihead[i] = ohead[i] = -1;
	rep(i, n) itail[i] = otail[i] = -1;

	rep(i, n) id[i] = i;
}

int add_teleporter(int u, int v) {
	if(v <= u && u < k) return true;

	static int step = 0; step++;
	if(step == tofctv) scc(), step = 0;

	u = id[u], v = id[v];

	oprev[om] = ohead[u];
	oto[om] = v;
	ohead[u] = om++;
	if(otail[u] == -1) otail[u] = ohead[u];

	iprev[im] = ihead[v];
	ito[im] = u;
	ihead[v] = im++;
	if(itail[v] == -1) itail[v] = ihead[v];

	return dfs(v, a[u], inf) || dfs(u, -1,  b[v]);
}
