#include "game.h"

#include <bits/stdc++.h>

using namespace std;

#define FOR(i,a,b) for(int i=int(a);i<int(b);i++)
#define REP(i,b) FOR(i,0,b)

using vi=vector<int>;
using pi=pair<int,int>;

#define PB push_back

const int Nmax=300010;

vi g[Nmax];

int n,k,reach[Nmax];

void init(int N, int K) {
	n=N;
	k=K;
	REP(i,k)
		reach[i]=i;
	FOR(i,k,n)
		reach[i]=-1;
}

bool dfs(int v,int x){
	if(v<k&&v<=x)return true;
	if(x<=reach[v])return false;
	reach[v]=x;
	for(auto to:g[v])
		if(dfs(to,reach[v]))return true;
	return false;
}

int add_teleporter(int u, int v){
	if(max(u,v)<k){
		if(u<v)return 0;
		else return 1;
	}
	g[u].PB(v);
	return dfs(v,reach[u]);
}
