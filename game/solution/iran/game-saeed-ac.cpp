#include "game.h"

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

const int MAXN = 1000000;
const int MAXE = 1000000;

bool found;
int k, m;
int in[MAXN], out[MAXN], last[MAXN];
int e1[MAXE], e2[MAXE], edge_level[MAXE];
vector<int> lazy[MAXN];

void go (int index, int lo, int hi, int lev) {
    if (lo > hi || edge_level[index] != lev)
        return;
    int u = e1[index], v = e2[index], mid = (lo + hi)/2;
    in[u] = min(in[u], in[v]);
    out[v] = max(out[u], out[v]);
    int mask_u = ((in[u] <= mid) << 1) + (mid <= out[u]);
    int mask_v = ((in[v] <= mid) << 1) + (mid <= out[v]);
    if (mask_u == 0 || mask_v == 0) {
        if (mask_u == 0) {
            lazy[u].push_back(index);
            last[u] = lev;
        }
        if (mask_v == 0) {
            lazy[v].push_back(index);
            last[v] = lev;
        }
        return;
    }
    int t = -1;
    if (last[u] == lev)
        t = u;
    if (last[v] == lev)
        t = v;
    vector<int> old_edges;
    if (t != -1) {
        old_edges = lazy[t];
        lazy[t].clear();
        last[t] = -1;
    }
    edge_level[index]++;
    if ((mask_v & 2) && (mask_u & 1))
        found = true;
    else if (mask_u == 1 && mask_v == 1)
        go(index, mid+1, hi, lev+1);
    else if (mask_u == 2 && mask_v == 2)
        go(index, lo, mid-1, lev+1);
    for (int i = 0; i < (int)old_edges.size(); i++)
        go(old_edges[i], lo, hi, lev);
}

bool add_portal(int u, int v) {
    e1[m] = u, e2[m] = v;
    m++;
    if (!found)
        go(m-1, 0, k-1, 0);
    return found;
}

void init(int n, int k) {
    ::k = k;
    for (int i = 0; i < n; i++)
        if (i < k)
            in[i] = out[i] = i, last[i] = -1;
        else
            in[i] = k, out[i] = -1, last[i] = -1;
}
