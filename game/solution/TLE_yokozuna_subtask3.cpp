#include "game.h"
#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>
using namespace std;

const int MAX_N = 30010;
const int MAX_K = 50010;

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

int cnt = 0;
static bool used[MAX_N];
static int hoge[MAX_N+10];
int hoge_k = 0;
int hoge_l = 0;
bool dfs_check(int v){
	for(int i: G[v])if(!used[i]){
		hoge[hoge_k++] = i;
		used[i] = true;
	}
	while(hoge_l < hoge_k){
		int q = hoge[hoge_l++];
		if(q == v)return true;
		for(int i: G[q]){
			if(!used[i]){
				hoge[hoge_k++] = i;
				used[i] = true;
			}
		}
	}
	return false;
}

bool add_portal(int u,int v){
	G[u].push_back(v);
	rG[v].push_back(u);
	bool ret = false;
	for(int i = 0 ; i < MAX_N+10 ; i ++)used[i] = false;
	hoge_k = hoge_l = 0;
	for(int i = K-1 ; i >= 0 ; i --){
		ret |= dfs_check(i);
	}
	return ret;
}

