#include "game.h"

#include <bits/stdc++.h>

using namespace std;

#define FOR(i,a,b) for(int i=int(a);i<int(b);i++)
#define REP(i,b) FOR(i,0,b)

using vi=vector<int>;
using pi=pair<int,int>;

#define PB push_back

const int Nmax=300010;
const int S=100000000;

vi g[Nmax];

int n,k;

pi buf[Nmax];
int bufUsed;
void Push(pi v){
	buf[bufUsed++]=v;
}
pi Pop(){
	return buf[--bufUsed];
}
bool Contain(){
	return bufUsed;
}

struct Sub{
	int passV;
	vector<bool> vis;
	void Init(int pv,int n){
		passV=pv;
		vis.resize(n,0);
		for(int i=passV;i<k;i++)
			for(auto j:g[i])
				enter(i,j);
	}
	bool enter(int u,int v){
		while(Contain())Pop();
		Push(pi(u,v));
		while(Contain()){
			pi e=Pop();
			u=e.first,v=e.second;
			if(u<k){
				if(u<passV)
					return false;
				else
					vis[u]=true;
			}
			if(v<k){
				if(passV<v)
					return false;
				else
					return vis[u];
			}
			if(vis[u]&&!vis[v]){
				vis[v]=true;
				for(auto to:g[v]){
					if(to==passV)
						return true;
					if(!vis[to])
						Push(pi(v,to));
				}
			}
		}
		return false;
	}
};

vector<Sub> ss;
bool used[Nmax];

void init(int N, int K) {
	n=N;
	k=K;
}

int add_teleporter(int u, int v){
	if(max(u,v)<k){
		if(u<v)return 0;
		else return 1;
	}
	static int cnt=0;
	if(u<k&&!used[u]&&(cnt++)*n<S){
		used[u]=true;
		ss.emplace_back();
		ss.back().Init(u,n);
	}
	g[u].PB(v);
	for(auto&s:ss)
		if(s.enter(u,v))return 1;
	return 0;
}
