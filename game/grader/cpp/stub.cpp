#include <cstdio>
#include <csignal>
#include <cstdlib>
#include "game.h"

static int N,K;
//static FILE *in,*out;

int main(int argc,char **argv)
{
	/*if(argc!=3)
	{
		fprintf(stderr, "Usage: %s <out> <in>\n", argv[0]);
		return 1;
	}*/
	signal(SIGPIPE, SIG_IGN);
	//in=stdin;//fopen(argv[2],"r");
	//out=stdout;//fopen(argv[1],"w");
	
	int scanRes;
	(void)scanRes;
	
	scanRes=scanf("%d %d",&N,&K);
	init(N,K);
	
	while(1)
	{
		int block;
		scanRes=scanf("%d",&block);
		if(block==-1) break;
		
		int i=0;
		for(;i<block;i++)
		{
			int u,v;
			scanRes=scanf("%d %d",&u,&v);
			int res=add_teleporter(u^893123,v^893123);
			if(res!=0&&res!=1){
				i=-1;
				break;
			}else if(res==1) break;
		}
		printf("%d\n",i);
		fflush(stdout);
		if(i!=block) break;
	}
	
	return 0;
}
