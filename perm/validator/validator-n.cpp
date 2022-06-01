#include "testlib.h"
using namespace std;
const int MAXT = 100;
const long long MAXK = 90;
int main()
{
	registerValidation();
	string secret = inf.readLine();
	ensuref(secret == (string)"a92b3f80-b312-8377-273c-3916024d7f2a", "Secret not found!");
	int t = inf.readInt(1, MAXT, "t");
	inf.readEoln();
	for (int i = 0; i < t; i++)
	{
		inf.readLong(2, MAXK, "k");
		inf.readEoln();
	}
	inf.readEof();
	return 0;
}
