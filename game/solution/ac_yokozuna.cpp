#include "game.h"
#include <vector>
#include <iostream>
#include <cstdio>
using namespace std;

const int MAX_N = 300000;
const int MAX_K = 500000;

static int N,K;
static int l[MAX_N];
static int r[MAX_N];
static vector<int> G[MAX_N];
static vector<int> rG[MAX_N];

void init(int n,int k){
	N = n;
	K = k;
	for(int i = 0 ; i < K ; i ++){
		l[i] = i;
		r[i] = i;
	}
	for(int i = K ; i < N ; i++){
		l[i] = -1;
		r[i] = K;
	}
}

bool rupdate(int u,int v);

bool update(int u,int v){
	if(r[v] <= l[u])return true;
	if(r[u] <= l[v])return false;
	bool b = false;
	while((l[v]+r[v]+1)/2 <= l[u]){
		b = true;
		l[v] = (l[v]+r[v]+1)/2;
	}
	bool ret = false;
	if(b){
		for(int i = 0 ; i < G[v].size() ; i ++){
			ret |= update(v,G[v][i]);
			ret |= rupdate(v,G[v][i]);
		}
		for(int i = 0 ; i < rG[v].size() ; i ++){
			ret |= update(rG[v][i],v);
			ret |= rupdate(rG[v][i],v);
		}
	}
	return ret;
}

bool rupdate(int u,int v){
	if(r[v] <= l[u])return true;
	if(r[u] <= l[v])return false;
	bool b = false;
	while((l[u]+r[u]+2)/2-1 >= r[v]){
		b = true;
		r[u] = (l[u]+r[u]+2)/2-1;
	}
	bool ret = false;
	if(b){
		for(int i = 0 ; i < rG[u].size() ; i ++){
			ret |= update(rG[u][i],u);
			ret |= rupdate(rG[u][i],u);
		}
		for(int i = 0 ; i < G[u].size() ; i ++){
			ret |= update(u,G[u][i]);
			ret |= rupdate(u,G[u][i]);
		}
	}
	return ret;
}

bool add_portal(int u,int v){
	if(0 <= u && u < K && 0 <= v && v < K)return u >= v;
	G[u].push_back(v);
	rG[v].push_back(u);
	if(0 <= u && u < K){
		return update(u,v);
	}
	else if(0 <= v && v < K){
		return rupdate(u,v);
	}
	bool ret = false;
	ret |= update(u,v);
	ret |= rupdate(u,v);
	
	return ret;
}

int add_teleporter(int u,int v){
	if(add_portal(u,v))return 1;
	return 0;
}
