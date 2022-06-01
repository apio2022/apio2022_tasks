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

bool update(int u,int v);
bool rupdate(int u,int v);
bool update(int u,int v){
	bool ret = false;
	if(l[u] > l[v]){
		l[v] = l[u];
		if(l[v] >= r[v])return true;
		for(int i: G[v])ret |= update(v,i);
		for(int i: rG[v])ret |= update(i,v);
	}
	return ret;
}
bool rupdate(int u,int v){
	bool ret = false;
	if(r[v] < r[u]){
		r[u] = r[v];
		if(l[u] >= r[u])return true;
		for(int i: G[u])ret |= update(u,i);
		for(int i: rG[u])ret |= update(i,u);
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

