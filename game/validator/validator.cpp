#include "testlib.h"
#include <set>
#include <iostream>
using namespace std;
const int max_n = 300000;
const int max_m = 500000;

int main(int argc, char *argv[])
{
	registerValidation();
	int n = inf.readInt(1, max_n, "n");
	inf.readSpace();
	int m = inf.readInt(1, max_m, "m");
	inf.readSpace();
	int k = inf.readInt(1, n, "k");
	inf.readEoln();
	set < pair<int,int> > edges;
	for (int i = 0; i < k-1; i++) edges.insert(make_pair(i, i+1));
	while (m--)
	{
		int u = inf.readInt(0, n - 1, "u_i");
		inf.readSpace();
		int v = inf.readInt(0, n - 1, "v_i");
		ensuref(edges.count(make_pair(u, v)) == 0, "The given graph cannot contain multiple edges");
        edges.insert(make_pair(u, v));
		inf.readEoln();
	}
    inf.readEof();
	return 0;
}
