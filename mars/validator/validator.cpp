#include "testlib.h"
using namespace std;

const int N=41,MXT=10,MXK=20;

int dx[4]={1,0,-1,0};
int dy[4]={0,1,0,-1};

int n,k;
string gr[N];
int vis[N][N];

int go(int x,int y,char inv,int pa=-1)
{
	if(x<0||y<0||x>=n||y>=n||gr[x][y]==inv) return 0;
	if(vis[x][y]) return 1;
	int res=0;
	for(int d=0;d<4;d++)
	{
		if((d^2) == pa) continue;
		res|=go(x+dx[d],y+dy[d],inv,d);
	}
	return res;
}


int main(int argc, char *argv[])
{
  registerValidation();
  int sub=atoi(argv[1]);
  int t=inf.readInt(1,MXT); inf.readEoln();
  while(t--)
  {
    k=inf.readInt(1,MXK); inf.readEoln();
    n=2*k+1;
    if(sub!=0) ensuref(k<=2*sub,"N <= mxn");
    for(int i=0;i<n;i++)
    {
    	gr[i]="";
      for(int j=0;j<n;j++)
      {
      	string tmp=inf.readToken("[01]{1,1}");
      	gr[i]+=tmp;
      	if(j==n-1) inf.readEoln();
      	else inf.readSpace();	
      }
    }
    
  }
  inf.readEof();
  return 0;
}
