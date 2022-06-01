#include "testlib.h"
using namespace std;

static string output_secret = "6cad0f33-b1bd-3a3e-1a8d-c4af23adfcbf";

const long long MX=1e18;
long long bit[5005];
int mx;

void init(int n)
{
	for(int i=0;i<=n+1;i++)
		bit[i]=0;
	mx=n+1;
}

long long add(long long x,long long y)
{
	x+=y;
	if(x>MX) return MX+1;
	return x;
}

void update(int ind,long long x)
{
	ind++;
	for(;ind<=mx;ind+=ind&(-ind))
		bit[ind]=add(bit[ind],x);
}

long long query(int ind)
{
	long long ret=0;
	ind++;
	for(;ind>0;ind-=ind&(-ind))
		ret=add(ret,bit[ind]);
	return ret;
}

long long getInc(vector<int> perm)
{
	int n=perm.size();
	init(n);
	long long ans=1;
	update(0,1);
	for(int i=0;i<n;i++)
	{
		long long me=query(perm[i]+1);
		update(perm[i]+1,me);
		ans=add(ans,me);
	}
	return ans;
}

int main(int argc, char * argv[])
{
	registerChecker("perm", argc, argv);

	readBothSecrets(output_secret);
	readBothGraderResults();

	inf.readToken();
	int t=inf.readInt();
	int mxn=0;
	while(t--)
	{
		long long k=inf.readLong();
		int n=ouf.readInt(1,5000);
		vector<int> perm(n);
		set<int> ss;
		for(int i=0;i<n;i++)
		{
			perm[i]=ouf.readInt(0,n-1);
			ss.insert(perm[i]);
		}
		if(ss.size()!=n)
			quitf(_wa,"");
		long long g=getInc(perm);
		if(g!=k)
			quitf(_wa,"");
		mxn=max(mxn,n);
	}
	if(mxn<=90)
		quitf(_ok,"");
	else if(mxn<=120)
		quitp((90-(mxn-90)/3.0)/90.0,"");
	else
		quitp((80-(mxn-120)/65.0)/90.0,"");
}
