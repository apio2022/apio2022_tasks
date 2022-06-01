#include "testlib.h"
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char * argv[])
{
	registerChecker("mars", argc, argv);

	int t=inf.readInt();
	int k=inf.readInt();
	cerr << "K = " << k << ", ";
	int o=ouf.readInt();
	while(!ouf.seekEof()) ouf.readToken();
	if(o==1)
		quitf(_ok,"Correct");
	else if(o==0)
		quitf(_wa,"Incorrect");
	else
		quitf(_fail,"Fail");
}
