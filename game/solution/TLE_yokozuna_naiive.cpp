//–ˆ‰ñ‘S•”‚É‚Â‚¢‚Äo—ˆ‚é‚©‚Ç‚¤‚©’²‚×‚éB
//O(m(m+k))?
#include "game.h"
#include <vector>
#include <queue>
#include <algorithm>
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

bool dfs_check(int v){
	bool used[MAX_N];
	for(int i = 0 ; i < MAX_N ; i ++){
		used[i] = false;
	}
	queue<int> que;
	que.push(-v);
	while(!que.empty()){
		int q = que.front();
		if(q == v)return true;
		q = abs(q);
		for(int i = 0 ; i < G[q].size() ; i ++){
			if(!used[G[q][i]]){
				que.push(G[q][i]);
				used[G[q][i]] = true;
			}
		}
	}
	return false;
}

bool add_portal(int u,int v){
	G[u].push_back(v);
	rG[v].push_back(u);
	bool ret = false;
	for(int i = 0 ; i < K ; i ++){
		ret |= dfs_check(i);
	}
	return ret;
}

