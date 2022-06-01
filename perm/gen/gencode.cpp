#include <iostream>
#include "testlib.h"
using namespace std;

vector<int> primes={2,3,5,7,11,13,17,19,23,29,31,37,39,41};

const long long MX=1e18;

long long genBits(int bits,int ones)
{
	long long res=0;
	set<int> ss;
	while(ss.size()<ones)
	{
		int x=rnd.next(0,bits-1);
		res|=(1LL<<x);
		ss.insert(x);
	}
	return max(res,2LL);
}

long long genComp()
{
	long long cur=1;
	int x=rnd.next(5,30);
	for(int i=0;i<x;i++)
	{
		int r=rnd.next(0,(int)primes.size()-1);
		if(cur<MX/primes[r])
			cur*=primes[r];
	}
	return cur;
}

long long getMaxBase(int b)
{
	long long ret=1;
	while(ret <= MX/b) ret*=b;
	return ret-1;
}

int main(int argc, char *argv[])
{
	registerGen(argc, argv);
	cout << "a92b3f80-b312-8377-273c-3916024d7f2a" << endl;
	string type(argv[1]);
	if (type == "subtask1")
	{
		cout << 89 << endl;
		for(int i=2;i<=90;i++)
			cout << i << endl;
	}
	if (type == "random")
	{
		int limp=atoi(argv[2]);
		long long lim=1;
		for(int i=0;i<limp;i++)
			lim*=10;
		cout << 100 << endl;
		for(int i=0;i<100;i++)
			cout << rnd.next(1LL,lim) << endl;
	}
	if (type == "consecutive")
	{
		long long st = atoll(argv[2]);
		cout << 100 << endl;
		for(long long i=st;i<st+100;i++)
			cout << i << endl;
	}
	if (type == "nasty")
	{
		int bits = atoi(argv[2]);
		cout << 100 << endl;
		int cur=0;
		for(int i=bits;i>=max(2,bits-10);i--)
		{
			cur++;
			cout << (1LL<<(i+1))-1 << endl;
		}
		while(cur<100)
		{
			cur++;
			int nb=rnd.next(2,bits);
			cout << genBits(nb,rnd.next(nb/2,nb)) << endl;
		}

	}
	if(type=="composite")
	{
		cout << 100 << endl;
		for(int i=0;i<100;i++)
			cout << genComp() << endl;
	}
	if(type=="maxbase")
	{
		cout << 100 << endl;
		for(int i=3;i<103;i++)
		{
			cout << getMaxBase(i) << endl;
		}
	}
	return 0;
}
