#include "testlib.h"
#include <set>
#include <iostream>
using namespace std;
const int max_n = 1000;
const int max_m = 5000;

int main(int argc, char *argv[])
{
	registerValidation();
	inf.readInt(1, max_n, "n");
	inf.readSpace();
	int m = inf.readInt(1, max_m, "m");
	skip_ok();

	return 0;
}
