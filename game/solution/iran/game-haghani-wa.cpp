#include <iostream>
#include <set>
using namespace std;
const int MAX = 500005;
set<int> adj[MAX], radj[MAX];
int state[20][MAX], k;
int uq[MAX], vq[MAX], h, t;
bool upd(int v, int s, int lvl, set<int> *adj, bool inv = false)
{
	if (state[lvl][v] == s)
		return false;
	if (!state[lvl][v])
		state[lvl][v] = s;
	else
		return true;
	for (set<int>::iterator it = adj[v].begin(); it != adj[v].end(); it++)
	{
		uq[t] = v;
		vq[t++] = *it;
		if (inv)
			swap(uq[t - 1], vq[t - 1]);
	}
	adj[v].clear();
	return false;
}
bool add(int u, int v, int l = 0, int r = k, int lvl = 0)
{
	if (!state[lvl][u] && !state[lvl][v])
	{
		if (lvl == 0)
		{
			adj[u].insert(v);
			radj[v].insert(u);
		}
		return false;
	}
	int mid = (l + r) / 2;
	state[lvl][mid] = 3;
	if (u == mid || state[lvl][u] == 2)
	{
		if (upd(v, 2, lvl, adj))
			return true;
		else if (u != mid)
		{
			return add(u, v, mid, r, lvl + 1);
		}
	}
	if (v == mid || state[lvl][v] == 1)
	{
		if (upd(u, 1, lvl, radj, true))
			return true;
		else if (v != mid)
			return add(u, v, l, mid, lvl + 1);
	}
	if (state[lvl][u] == 1)
		return add(u, v, l, mid, lvl + 1);
	return add(u, v, mid, r, lvl + 1);
}
bool add_portal(int u, int v)
{
	h = 0, t = 0;
	uq[t] = u;
	vq[t++] = v;
	while (h < t)
	{
		if (add(uq[h], vq[h]))
			return true;
		h++;
	}
	return false;
}
void make_threes(int l, int r, int lvl)
{
	if (l == r)
		return;
	int mid = (l + r) / 2;
	state[lvl][mid] = 3;
	make_threes(l, mid, lvl + 1);
	make_threes(mid + 1, r, lvl + 1);
}
void init(int n, int _k)
{
	k = _k;
	make_threes(0, k, 0);
	for (int i = 0; i < k - 1; i++)
		add_portal(i, i + 1);
}

int add_teleporter(int u,int v){
	return add_portal(u,v);
}
