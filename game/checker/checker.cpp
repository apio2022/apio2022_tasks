#include "testlib.h"
using namespace std;

int main(int argc, char * argv[])
{
	registerChecker("game", argc, argv);

	compareRemainingLines(1);
}
