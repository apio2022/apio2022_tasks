#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#include "game.h"
#define SIZE 300005

using namespace std;

vector <int> vec[SIZE];
vector <int> rvec[SIZE];
int left[SIZE],right[SIZE];
queue <int> que;
int mum;

void init(int n, int k)
{
	mum=1;
	while(mum<k+2) mum<<=1;
	for(int i=0;i<k;i++) left[i]=i+1,right[i]=i+2;
	for(int i=k;i<n;i++) left[i]=0,right[i]=mum;
	for(int i=0;i<n;i++)
	{
		vec[i].clear();
		rvec[i].clear();
	}
	while(!que.empty()) que.pop();
}
bool check(int u,int v)
{
	if(right[v]<=left[u]) return true;
	if((left[v]+right[v])/2<=left[u])
	{
		left[v]=(left[v]+right[v])/2;
		que.push(v);
	}
	if(right[v]<=(left[u]+right[u])/2)
	{
		right[u]=(left[u]+right[u])/2;
		que.push(u);
	}
	return false;
}
int add_teleporter(int u, int v)
{
	vec[u].push_back(v);
	rvec[v].push_back(u);
	if(check(u,v)) return 1;
	while(!que.empty())
	{
		int x=que.front();que.pop();
		if(left[x]+1==right[x]) return 1;
		for(int i=0;i<vec[x].size();i++)
		{
			int to=vec[x][i];
			if(check(x,to)) return 1;
		}
		for(int i=0;i<rvec[x].size();i++)
		{
			int to=rvec[x][i];
			if(check(to,x)) return 1;
		}
	}
	return 0;
}
