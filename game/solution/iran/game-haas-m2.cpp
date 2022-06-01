#include "game.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

const int max_n = 300010;

int n, k;
vector <int> a[max_n], b[max_n];

int mark[max_n], tc;
vector <int> topol;
bool path, not_path;

void dfs(int v, vector<int> *graph, bool is_topol=false) {
    (v < k ? path : not_path) = true;
    mark[v] = tc;
    for (auto u : graph[v])
        if (mark[u] != tc)
            dfs(u, graph, is_topol);
    if (is_topol)
        topol.push_back(v);
}

bool add_portal(int u, int v) {
	a[u].push_back(v);
	b[v].push_back(u);
	if (u < k && v < k)
		return u >= v;

    topol.resize(0);
    tc++;
    for (int i = 0; i < n; i++)
        if (mark[i] != tc)
            dfs(i, a, true);
    reverse(topol.begin(), topol.end());
    tc++;
    for (auto x : topol)
        if (mark[x] != tc) {
            path = not_path = false;
            dfs(x, b);
            if (path && not_path) return true;
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
