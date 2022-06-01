#include <bits/stdc++.h>

using namespace std;

const int INSTANCES=2,N=35,CASES=10;
const long long MXA=1e15;

mt19937 rng(19381);

int getRand(int x,int y)
{
  return uniform_int_distribution<int>(x, y)(rng);
}

FILE *fin[INSTANCES];
FILE *fout[INSTANCES];

vector<string> grid[CASES];
vector<vector<long long> > cur[CASES],nxt[CASES];
int n[CASES],i[CASES],j[CASES],k[CASES];
int ans[CASES];
int t;

inline void finish() {
  for(int c=0;c<t;c++)
  {
    if(cur[c].size()==1 && cur[c][0].size()==1)
      cout << cur[c][0][0] << endl;
    else
      cout << -1 << endl;
  }
}

inline void doubleWrite(string code, string msg) {
  if(code=="OK")
    cout << 1.0 << endl << msg << endl;
  else
    cout << 0 << endl << msg << endl;
#ifdef ERRLOG
  cerr << code << endl << msg << endl;
#endif
}

inline void die(string code, string msg,bool sendKill=1) {
    if(sendKill)
    {
      for(int i=0;i<INSTANCES;i++)
      {
        fprintf(fout[i], "%d %d %d %d\n", -1, -1, -1, -1);
        fflush(fout[i]);
      }
    }
    if(code=="OK")
    {
      for(int c=0;c<t;c++)
      {
        if(cur[c].size()!=1 || cur[c][0].size()!=1 || cur[c][0][0]!=ans[c])
        {
          code="WA";
        }
      }
    }
    cerr << "K = " << k[0] << ", " << code << endl;
    doubleWrite(code, msg);
    finish();
    exit(0);
}

const string OK ="OK";
const string WA = "WA";
const string FAIL = "FAIL";

long long goAsk(int c)
{
  int curin=getRand(0,INSTANCES-1);
  fprintf(fout[curin], "%d %d %d %d\n",i[c],j[c],n[c],k[c]);
  for(int x=0;x<3;x++)
  {
    for(int y=0;y<3;y++)
    {
      fprintf(fout[curin], "%lld%c",cur[c][i[c]+x][j[c]+y]," \n"[y+1==3]);
    }
  }
  fflush(fout[curin]);
  long long ret;
  if(fscanf(fin[curin],"%lld",&ret)!=1)
  {
    die(WA,"Didn't recieve a return from grader",0);
  }
  if(ret<0||ret>MXA)
    die(WA,"Invalid return value");
  return ret;
}

int goNext(int c)
{
  // 0 - finished | 1- go to next n | 2-nothing
  int lasn=2*(k[c]-n[c])-1;
  if(n[c]==k[c]) return 0;
  j[c]++;
  if(j[c]==lasn)
  {
    i[c]++; j[c]=0;
  }
  if(i[c]==lasn)
  {
    i[c]=0; n[c]++;
    if(n[c]==k[c]) return 0;
    return 1;
  }
  return 2;
}

void go(int x,int y,vector<string> &gr,vector<vector<int> > &vis)
{
  if(x<0||y<0||x>=gr.size()||y>=gr.size()||gr[x][y]=='0') return;
  if(vis[x][y]) return;
  vis[x][y]=1;
  for(int dx=-1;dx<=1;dx++)
  {
    for(int dy=-1;dy<=1;dy++)
    {
      if(abs(dx)+abs(dy)!=1) continue;
      go(x+dx,y+dy,gr,vis);
    }
  }
}

int calcAns(int c)
{
  vector<vector<int>> vis(2*k[c]+1,vector<int>(2*k[c]+1,0));
  int curans=0;
  for(int x=0;x<2*k[c]+1;x++)
  {
    for(int y=0;y<2*k[c]+1;y++)
    {
      if(grid[c][x][y]=='1' && !vis[x][y])
      {
        curans++;
        go(x,y,grid[c],vis);
      }
    }
  }
  return curans;
}

void readInput()
{
  scanf("%d",&t);
  for(int c=0;c<t;c++)
  {
    scanf("%d",&k[c]);
    grid[c]=vector<string>(2*k[c]+1,string(2*k[c]+1,'.'));
    cur[c]=vector<vector<long long> >(2*k[c]+1,vector<long long>(2*k[c]+1,0));
    nxt[c]=vector<vector<long long> >(2*k[c]-1,vector<long long>(2*k[c]-1,0));
    for(int x=0;x<2*k[c]+1;x++)
    {
      for(int y=0;y<2*k[c]+1;y++)
      {
        scanf(" %c",&grid[c][x][y]);
        cur[c][x][y]=grid[c][x][y]-'0';
      }
    }
    ans[c]=calcAns(c);
  }
}

int main(int argc, char **argv) {
  if(argc<1+2*INSTANCES)
  {
    die(FAIL,"ERROR",0);
  }
  for(int i=0;i<INSTANCES;i++)
  {
    fout[i] = fopen(argv[2+i*2], "a");
    fin[i] = fopen(argv[1+i*2], "r");
  }
  readInput();
  vector<int> notFin(t);
  iota(notFin.begin(),notFin.end(),0);
  while(notFin.size())
  {
    int x=getRand(0,notFin.size()-1);
    int c=notFin[x];
    long long ret=goAsk(c);
    nxt[c][i[c]][j[c]]=ret;
    int res=goNext(c);
    if(res==0)
    {
      swap(cur[c],nxt[c]);
      swap(notFin[x],notFin.back());
      notFin.pop_back();
    }
    else if(res==1)
    {
      swap(cur[c],nxt[c]);
      nxt[c]=vector<vector<long long> >(2*(k[c]-n[c])-1,vector<long long>(2*(k[c]-n[c])-1,0));
    }
  }
  die(OK,"ok");
  return 0;
}
