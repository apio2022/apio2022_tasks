#include "game.h"
#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>
using namespace std;

const int MAX_N = 300000;
const int MAX_K = 500000;

static int N,K;
static vector<int> G[MAX_N];
static vector<int> rG[MAX_N];

void init(int n,int k){
	N = n;
	K = k;
	for(int i = 0 ; i+1 < k ; i ++){
		G[i].push_back(i+1);
		rG[i+1].push_back(i);
	}
}

static bool used[MAX_N];
bool dfs_check(int v){
	queue<int> que;
	for(int i: G[v])que.push(i);
	while(!que.empty()){
		int q = que.front(); que.pop();
		if(q == v)return true;
		for(int i: G[q]){
			if(!used[i]){
				que.push(i);
				used[i] = true;
			}
		}
	}
	return false;
}

int cnt = 0;
bool add_portal(int u,int v){
	G[u].push_back(v);
	rG[v].push_back(u);
	bool ret = false;
	for(int i = 0 ; i < 2010 ; i ++)used[i] = false;
	for(int i = K-1 ; i >= 0 ; i --){
		ret |= dfs_check(i);
	}
	return ret;
}

