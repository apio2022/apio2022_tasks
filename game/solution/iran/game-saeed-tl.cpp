#include "game.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

const int MAXN = 1000000;
const int RANGE_ABS_LIMIT = 0;
const double RANGE_REL_LIMIT = 0.8;
const int LAZY_LIMIT = 250000000;
const int INF = 1e9;

bool found;
int n, k, id, lazy;
int in[MAXN], out[MAXN], mark[MAXN];
vector<int> adj[MAXN], bak[MAXN];

bool update_all(int begin, int end, int *att, vector<int> g[]) {
    int sign = begin < end ? 1 : -1;
    id++;
    while (begin != end) {
        if (mark[begin] == id)
            return true;
        queue<int> q;
        q.push(begin);
        mark[begin] = id;
        att[begin] = begin;
        while (!q.empty()) {
            int front = q.front();
            q.pop();
            for (int i = 0; i < (int)g[front].size(); i++) {
                int temp = g[front][i];
                if (mark[temp] != id) {
                    q.push(temp);
                    mark[temp] = id;
                    att[front] = begin;
                }
            }
        }
        begin += sign;
    }
    return false;
}

int counter;

bool propagate(int u, int new_in, int new_out) {
    int old_in = in[u], old_out = out[u];
    in[u] = min(in[u], new_in);
    out[u] = max(out[u], new_out);
    if (in[u] == old_in && out[u] == old_out)
        return false;
    if ((u >= k && in[u] == out[u]) || (in[u] < out[u]))
        return found = true;
    if ((in[u] - out[u]) > RANGE_ABS_LIMIT && (in[u] - out[u]) > (old_in - old_out) * RANGE_REL_LIMIT)
        return false;
    if (old_in != in[u]) {
        for (int i = 0; i < (int)bak[u].size(); i++)
            if (propagate(bak[u][i], new_in, -INF))
                return true;
    }
    if (old_out != out[u]) {
        for (int i = 0; i < (int)adj[u].size(); i++)
            if (propagate(adj[u][i], INF, new_out))
                return true;
    }
    return false;
}

bool add_portal(int u, int v) {
    counter++;
    if(counter % 2000 == 0)
        cerr << " >>> " << counter << endl;
    if (found || (v <= u && u < k))
        return found = true;
    adj[u].push_back(v);
    bak[v].push_back(u);
    lazy++;
    if (lazy == LAZY_LIMIT) {
        if (update_all(0, k, in, bak) || update_all(k-1, -1, out, adj))
            return found = true;
        for (int i = k; i < n; i++) if (in[i] <= out[i])
            return found = true;
        lazy = 0;
    } else {
        id++;
        if (lazy > 268000)
            cerr << "BEGIN " << counter << endl;
        if (propagate(u, in[v], -INF) || propagate(v, INF, out[u]))
            return true;
        if (lazy > 268000)
            cerr << "END " << counter << endl;
    }
    return false;
}

void init(int _n, int _k) {
    n = _n, k = _k;
    for (int i = 0; i < k-1; i++) {
        adj[i].push_back(i+1);
        bak[i+1].push_back(i);
    }
    for (int i = 0; i < n; i++) {
        if (i < k)
            in[i] = out[i] = i;
        else
            in[i] = INF, out[i] = -INF;
    }
}

int add_teleporter(int u,int v){
	return add_portal(u,v);
}
