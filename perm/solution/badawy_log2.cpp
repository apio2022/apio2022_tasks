#include <bits/stdc++.h>
#include "perm.h"
using namespace std;

vector<int> add(vector<int> a,vector<int> b)
{
	for(int &i:a)
		i+=b.size();
	for(int i:b)
		a.push_back(i);
	return a;
}

vector<int> get(int p)
{
	vector<int> v;
	for(int i=0;i<p;i++)
	{
		v.push_back(i);
	}
	return v;
}

vector<int> construct_permutation(long long k) {
	vector<int> cur;
	for(int i=0;i<60;i++)
	{
		if(k&(1LL<<i))
			cur=add(cur,get(i));
	}
	for(int i=0;i+1<__builtin_popcountll(k);i++)
	{
		cur=add({0},cur);
	}
	return cur;
}

