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

bool vis[Nmax][2];

void dfs(int v,int t){
	if(vis[v][t])return;
	vis[v][t]=true;
	for(auto to:g[v])
		dfs(to,to<k?1:t);
}

int add_teleporter(int u, int v){
	if(max(u,v)<k){
		if(u<v)return 0;
		else return 1;
	}
	g[u].PB(v);
	
	memset(vis,0,sizeof(vis));
	dfs(v,0);
	return vis[v][1];
}
