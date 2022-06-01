#include <csignal>
#include <cstdlib>
#include <cstdio>
#include <algorithm>

void wrong(int num)
{
	fprintf(stderr, "translate:wrong\n");
	fprintf(stderr, "Wrong Answer [%d]\n", num);
	fprintf(stdout, "%.2lf\n", 0.0);
	exit(0);
}

const int MAX_N=300005;
const int MAX_M=500005;
int A[MAX_M],B[MAX_M];

int main(int argc, char** argv)
{
	int r_f=0;
	if (argc <3)
	{
		fprintf(stderr, "Usage: %s <in0> <out0>\n", argv[0]);
		return 1;
	}
	signal(SIGPIPE, SIG_IGN);
	FILE* in0 = fopen(argv[2], "w");
	FILE* out0 = fopen(argv[1], "r");
	// read testcase
	int n,m,k;
	scanf("%d %d %d",&n,&m,&k);
	for(int i=0;i<m;i++) scanf("%d %d",&A[i],&B[i]);
	// write to user program 0
	fprintf(in0, "%d %d\n",n,k);
	fflush(in0);
	
	int block=80;
	
	for(int i=0;i<m;i+=block)
	{
		int l=i,r=std::min(i+block,m);
		int sz=r-l;
		fprintf(in0,"%d\n",sz);
		for(int j=l;j<r;j++)
		{
			fprintf(in0,"%d %d\n",A[j]^893123,B[j]^893123);
		}
		fflush(in0);
		int t;
		if(fscanf(out0,"%d",&t)!=1){
		  wrong(4);
		}
		
		if(r==m)
		{
			if(t!=sz-1)
			{
				if(t==sz)
				{
					fprintf(in0,"%d\n",-1);
					fflush(in0);
				}
				wrong(1);
				break;
			}
		}
		else
		{
			if(t!=sz)
			{
				wrong(1);
				break;
			}
		}
	}
	fprintf(in0,"%d\n",-1);
	fflush(in0);
	fprintf(stderr, "translate:success\n");
	fprintf(stderr, "Accepted\n");
	fprintf(stdout, "%.2lf\n", 1.0);
	return 0;
}
