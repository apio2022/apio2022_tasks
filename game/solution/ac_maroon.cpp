#include "game.h"

#include <bits/stdc++.h>

using namespace std;

#define FOR(i,a,b) for(int i=int(a);i<int(b);i++)
#define REP(i,b) FOR(i,0,b)

using vi=vector<int>;

#define PB push_back

const int Nmax=300010;
const int Lmax=20;

vi g[Nmax][2];

int n,k,maxL;
int pos[Nmax];
bool reach[Lmax][Nmax][2];

int Level(int x){
	return 31-__builtin_clz(x);
}

int Par(int x,int lv){
	int w=Level(x);
	if(w<lv)return -1;
	else return x>>(w-lv);
}

bool SameNode(int lv,int u,int v){
	int up=Par(pos[u],lv),vp=Par(pos[v],lv);
	return up!=-1&&up==vp;
}

void init(int N, int K) {
	n=N+2;
	k=K+2;
	maxL=0;
	while((1<<maxL)<k)maxL++;
	REP(i,k){
		pos[i]=(1<<maxL)+i;
		int x=i;
		for(int j=maxL-1;j>=0;j--){
			reach[j][i][(x&1)^1]=true;
			x>>=1;
		}
	}
	FOR(i,k,n)
		pos[i]=1;
}

bool dfs(int lv,int v,int t,vi& vs);

bool enter(int lv,int u,int v,int t,vi& vs){
	if(v<k)return false;
	if(SameNode(lv,u,v)&&reach[lv][u][t]&&!reach[lv][v][t])
		return dfs(lv,v,t,vs);
	return false;
}

bool dfs(int lv,int v,int t,vi& vs){
	assert(!reach[lv][v][t]);
	reach[lv][v][t]=true;
	if(reach[lv][v][t^1])return true;
	vs.PB(v);
	pos[v]=pos[v]*2+(t^1);
	for(auto to:g[v][t])
		if(enter(lv,v,to,t,vs))return true;
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
	vi vs;
	REP(lv,maxL){
		vi nx;
		if(enter(lv,u,v,0,nx))return 1;
		if(enter(lv,v,u,1,nx))return 1;
		for(auto v:vs)
			REP(t,2)
				for(auto from:g[v][t^1])
					if(enter(lv,from,v,t,nx))return 1;
		vs.swap(nx);
	}
	if(!vs.empty())return 1;
	else return 0;
}
