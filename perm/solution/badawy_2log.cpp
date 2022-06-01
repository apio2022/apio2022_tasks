#include <bits/stdc++.h>
#include "perm.h"
using namespace std;

vector<int> construct_permutation(long long k) {
	string bn="";
	while(k)
	{
		bn+=k%2+'0';
		k/=2;
	}
	reverse(bn.begin(),bn.end());
	vector<int> ret;
	int cur=0;
	for(int i=1;i<bn.size();i++)
	{
		ret.push_back(cur++);
		if(bn[i]=='1')
			ret.insert(ret.begin(),cur++);
	}
	return ret;
}
