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

int state[Nmax];
bool dfs(int v){
	if(state[v]!=0){
		if(state[v]==1&&v<k)return true;
		else return false;
	}
	state[v]=1;
	for(auto to:g[v])
		if(dfs(to))return true;
	state[v]=2;
	return false;
}

bool check(){
	memset(state,0,sizeof(state));
	return dfs(0);
}

int add_teleporter(int u, int v){
	if(max(u,v)<k){
		if(u<v)return 0;
		else return 1;
	}
	g[u].PB(v);
	return check();
}
