#include "perm.h"
#include <cstdio>
#include <vector>
#include <cassert>
// BEGIN SECRET
#include <string>
// END SECRET

using namespace std;
// BEGIN SECRET
static string input_secret = "a92b3f80-b312-8377-273c-3916024d7f2a";
static string output_secret = "6cad0f33-b1bd-3a3e-1a8d-c4af23adfcbf";
// END SECRET

int main() {
	// BEGIN SECRET
	char secret[1000];
	assert(1 == scanf("%s", secret));
	if (string(secret) != input_secret) {
		printf("%s\n", output_secret.c_str());
		printf("SV\n");
		return 0;
	}
	// END SECRET
	int t;
	assert(1 == scanf("%d", &t));
	vector<long long> ks;
	vector<vector<int> > ret;
	while(t--)
	{
		long long k;
		assert(1 == scanf("%lld",&k));
		ks.push_back(k);
	}
	for(int i=0;i<(int)ks.size();i++){
		ret.push_back(construct_permutation(ks[i]));
	}
	// BEGIN SECRET
	printf("%s\n", output_secret.c_str());
	printf("OK\n");
	// END SECRET
	for(int i=0;i<(int)ret.size();i++)
	{
		printf("%d\n",(int)ret[i].size());
		for(int j=0;j<(int)ret[i].size();j++)
		{
			printf("%d",ret[i][j]);
			if(j+1==(int)ret[i].size())
				printf("\n");
			else
				printf(" ");
		}
	}
	return 0;
}
