#include "testlib.h"
#include <set>
#include <iostream>
using namespace std;
const int max_n = 30000;
const int max_m = 50000;
const int max_k = 2000;

int main(int argc, char *argv[])
{
	registerValidation();
	inf.readInt(1, max_n, "n");
	inf.readSpace();
	int m = inf.readInt(1, max_m, "m");
	inf.readSpace();
	inf.readInt(1, max_k, "k");
	skip_ok();

	return 0;
}
