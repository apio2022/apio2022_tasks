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

static int l2[MAX_N];
static int r2[MAX_N];
static int B = 200;

static bool RET;
static bool flag[100010];

void init(int n,int k){
	N = n;
	K = k;
	for(int i = 0 ; i < K ; i ++){
		l[i] = i;
		r[i] = i;
		l2[i] = i/B;
		r2[i] = i/B;
	}
	for(int i = K ; i < N ; i++){
		l[i] = -1;
		r[i] = K;
		l2[i] = -1;
		r2[i] = K;
	}
	RET = false;
	for(int i = 0 ; i < 100010 ; i ++)flag[i] = false;
}

bool update(int u,int v);
bool rupdate(int u,int v);
bool update(int u,int v){
	bool ret = false;
	if(l[u] > l[v] && l2[u] == l2[v] && r2[u] == r2[v]){
		l[v] = l[u];
		if(l[v] >= r[v])return true;
		for(int i: G[v])ret |= update(v,i);
		for(int i: rG[v])ret |= update(i,v);
	}
	return ret;
}
bool rupdate(int u,int v){
	bool ret = false;
	if(r[v] < r[u] && l2[u] == l2[v] && r2[u] == r2[v]){
		r[u] = r[v];
		if(l[u] >= r[u])return true;
		for(int i: G[u])ret |= update(u,i);
		for(int i: rG[u])ret |= update(i,u);
	}
	return ret;
}

bool update2(int u,int v);
bool rupdate2(int u,int v);
bool update2(int u,int v){
	bool ret = false;
	if(l2[u] > l2[v]){
		l2[v] = l2[u];
		if(l2[v] > r2[v])RET = true;
		if(l2[v] >= r2[v])ret = true;
		for(int i: G[v])ret |= update2(v,i);
		for(int i: rG[v])ret |= rupdate2(i,v);
	}
	return ret;
}
bool rupdate2(int u,int v){
	bool ret = false;
	if(r2[v] < r2[u]){
		r2[u] = r2[v];
		if(l2[u] > r2[u])RET = true;
		if(l2[u] >= r2[u])ret = true;
		for(int i: G[u])ret |= update2(u,i);
		for(int i: rG[u])ret |= rupdate2(i,u);
	}
	return ret;
}

bool add_portal(int u,int v){
	if(0 <= u && u < K && 0 <= v && v < K)return u >= v;
	G[u].push_back(v);
	rG[v].push_back(u);
	
	bool ret = false;
	bool t = update2(u,v) | rupdate2(u,v);
	if(RET)return true;
	if(t && !flag[l2[u]]){
		flag[l2[u]] = true;
		for(int i = l2[u]*B ; i < min((l2[u]+1)*B,K) ; i ++){
			for(int j: G[i])ret |= update(i,j);
			for(int j: rG[i])ret |= rupdate(j,i);
		}
	}
	if(l2[u] == r2[u] && l2[v] == r2[v]){
		ret |= update(u,v);
		ret |= rupdate(u,v);
	}
	
	/*for(int i = 0 ; i < N ; i ++){
		printf("%d %d %d %d\n",l[i],r[i],l2[i],r2[i]);
	}
	puts("------------");*/
	
	return ret;
	
	/*if(0 <= u && u < K){
		return update(u,v);
	}
	else if(0 <= v && v < K){
		return rupdate(u,v);
	}
	bool ret = false;
	ret |= update(u,v);
	ret |= rupdate(u,v);
	
	return ret;*/
}

