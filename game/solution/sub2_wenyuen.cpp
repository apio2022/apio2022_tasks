#include "game.h"

#include <bits/stdc++.h>

using namespace std;

#define FOR(i,a,b) for(int i=int(a);i<int(b);i++)
#define REP(i,b) FOR(i,0,b)

using vi=vector<int>;

#define PB push_back

const int Nmax=300010;

int n,k;

vi g[Nmax];

void init(int N,int K){
	n=N;
	k=K;
	REP(i,k-1)
		g[i].PB(i+1);
}

bool vis[Nmax];
vector<int> vv;

void dfs(int v){
	if(vis[v])return;
    vv.push_back(v);
	vis[v]=true;
	for(auto to:g[v])
		dfs(to);
}

int add_teleporter(int u, int v){
    if(max(u,v)<k){
		if(u<v)return 0;
		else return 1;
	}
	g[u].PB(v);
	
	for (int i = 0; i < k; i++) {
        for (int j = 0; j < vv.size(); j++) vis[vv[j]] = false;
        vv.clear();
        for (int j = 0; j < g[i].size(); j++) dfs(g[i][j]);
        if (vis[i]) return 1;
    }
    return 0;
}
