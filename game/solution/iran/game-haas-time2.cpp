#include "game.h"
#include <iostream>
#include <vector>
#include <set>
#include <cassert>

using namespace std;

const int max_n = 300010;

typedef set<int> vset;
typedef vector<int> vlist;
typedef vector<pair<int,int>> elist;

int n, k;
vlist a[max_n], b[max_n];
vset in[max_n], out[max_n];
elist pools[max_n];

#define onpart(x, s, e) ((s <= x) && (x <= e)) 
#define onpath(x) onpart(x, 0, k-1)

void dfs(int x, vset &mark, vset &other, vlist *graph, elist &pool) {
	mark.insert(x);
	if (other.count(x) > 0) throw 1;
	for (auto y : graph[x]) {
		pool.push_back(make_pair(x, y));
		if (mark.count(y) == 0 && !onpath(y))
			dfs(y, mark, other, graph, pool);
	}
}

void update(int, int, int, int);

void go_on(int u, int v, int s, int e, elist &pool, bool inward) {
	for (auto edge : pool)
		if (inward)
			update(edge.second, edge.first, s, e);
		else
			update(edge.first, edge.second, s, e);
	pool.resize(0);
	update(u, v, s, e);
}

void update(int u, int v, int s, int e) {
	if (e < s) return;
	int mid = (s + e) / 2;
	if (out[mid].count(u) > 0 || onpart(u, mid, k-1))
		dfs(v, out[mid], in[mid], a, pools[mid]),
		go_on(u, v, mid+1, e, pools[mid], false);
	if (in[mid].count(v) > 0 || onpart(v, 0, mid))
		dfs(u, in[mid], out[mid], b, pools[mid]),
		go_on(u, v, s, mid-1, pools[mid], true);
}

bool add_portal(int u, int v) {
	a[u].push_back(v);
	b[v].push_back(u);
	if (onpath(u) && onpath(v))
		return u >= v;
	try {
		update(u, v, 0, k-1); 
	} catch (int x) {
		return true;
	}
	return false;
}

void init(int _n, int _k) {
	n = _n, k = _k;
	for (int i = 0; i < k-1; i++)
		assert(add_portal(i, i + 1) == false);
}


int add_teleporter(int u,int v){
	return add_portal(u,v);
}
