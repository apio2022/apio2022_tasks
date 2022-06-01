#include "game.h"

#include <bits/stdc++.h>

using namespace std;

#define FOR(i,a,b) for(int i=int(a);i<int(b);i++)
#define REP(i,b) FOR(i,0,b)

using vi=vector<int>;

#define PB push_back

const int Nmax=300010;

vi g[Nmax][2];

int n,k;
int reach[Nmax][2];

void init(int N, int K){
	n=N+2;
	k=K+2;
	REP(i,k)REP(t,2)
		reach[i][t]=i;
	FOR(i,k,n){
		reach[i][0]=0;
		reach[i][1]=k-1;
	}
}

bool dfs(int u,int v,int t){
	if(v<k)return false;
	int pre=reach[v][t];
	reach[v][t]=(t==0?(const int&(*)(const int&,const int&))max<int>:(const int&(*)(const int&,const int&))min<int>)(reach[v][t],reach[u][t]);
	if(reach[v][1]<=reach[v][0])return true;
	int x=pre^reach[v][t^1];
	int y=reach[v][t]^reach[v][t^1];
	if(__builtin_clz(x)<__builtin_clz(y))
		REP(s,2)
			for(auto to:g[v][s])
				if(dfs(v,to,s)) return true;
	return false;
}

int add_teleporter(int u, int v){
	if(u<k-2)u++;
	else u+=2;
	if(v<k-2)v++;
	else v+=2;
	
	if(max(u,v)<k){
		if(u<v)return 0;
		else return 1;
	}
	g[u][0].PB(v);
	g[v][1].PB(u);
	if(dfs(u,v,0))return 1;
	if(dfs(v,u,1))return 1;
	
	return 0;
}
