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
const int maxn = 500000 + 100;

int k;
vector<int> out[maxn], in[maxn];
int a[maxn], b[maxn];
int q[10*maxn], head, tail;

bool bfs(int s, int mn, int mx) {
	if(s < k) return false;
	int ts = a[s] ^ b[s];
	smax(a[s], mn), smin(b[s], mx);
	if(b[s] <= a[s]) return true;

	head = tail = 0;

	if(__builtin_clz(ts) != __builtin_clz(a[s] ^ b[s]))
		q[tail++] = s;

	while(head < tail) {
		int u = q[head++];
		for(int v: out[u]) {
			if(v < k) continue;
			int t = a[v] ^ b[v];
			smax(a[v], a[u]);
			if(b[v] <= a[v]) return true;
			if(__builtin_clz(t) != __builtin_clz(a[v] ^ b[v]))
				q[tail++] = v;
		}
		for(int v: in[u]) {
			if(v < k) continue;
			int t = a[v] ^ b[v];
			smin(b[v], b[u]);
			if(b[v] <= a[v]) return true;
			if(__builtin_clz(t) != __builtin_clz(a[v] ^ b[v]))
				q[tail++] = v;
		}
	}
	return false;
}

void init(int n, int _k) {
	k = _k;
	rep(i, k) a[i] = b[i] = i + 1;
	fer(i, k, n) a[i] = 0, b[i] = k + 1;
}

bool add_portal(int u, int v) {
	if(v <= u && u < k) return true;
	out[u].pb(v), in[v].pb(u);
	return bfs(v, a[u], inf) || bfs(u, -1, b[v]);
}

int add_teleporter(int u,int v){
	return add_portal(u,v);
}
