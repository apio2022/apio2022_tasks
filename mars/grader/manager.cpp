#include <bits/stdc++.h>

using namespace std;

const int INSTANCES=8,N=35,CASES=10;
const int ix=935,jx=391,kx=626,nx=426;
const string A[3][3]={{"1010111000011100001101110100011101000000010110011011101010001010000001011000010011000001111011111111", "1101011000101100110110100011110010000010000100001010001110101111010000100001110000001110110011001010", "1110111001011000110000110000011011100110001101000110011000100110010110010000010100010010001000010011"},{"0001000101000011100000000101101111010000110011101101001100101100011111010011011001011000111010010110", "0101110011100100000011001001111100110110001011111011001000111100000000010000100110100001010010010000", "0011011100110001001101010011100011100000100110110011110001011100110010110111100001111000100101111011"},{"0100110010100000000000011101110110000010101011111010111011100000110000010101000111100100001010001000", "1000111110010110100111001100000010011010000011001111001100110000010001111011111100101000010100101111", "0001001111100010010011101111110000010101101100011110101100000111100100101111000001001111011110000100"}};


mt19937 rng(19381);

int getRand(int x,int y)
{
  return uniform_int_distribution<int>(x, y)(rng);
}


FILE *fin[INSTANCES];
FILE *fout[INSTANCES];

const string OK ="translate:success";
const string WA = "translate:wrong";
const string SV = "Security Violation";
const string FAIL = "FAIL";

const string input_secret="224bb858-b13b-5e97-cbba-4a10b0455e79";
const string output_secret="f18fba32-f6de-4dd0-ef1b-ea027937a4aa";
const string sv_secret="dcc49c2d-df9d-dc50-3ba2-ef7527b9539b";

vector<string> grid[CASES];
vector<vector<string> > cur[CASES];
//int n[CASES],i[CASES],j[CASES],k[CASES];
int k[CASES],ans[CASES];
int t,mxk;

long long getnum(string s)
{
  for(int i=30;i<s.size();i++)
    if(s[i]=='1') return -1;
  long long ans=0;
  for(int i=s.size()-1;i>=0;i--)
    ans=(ans*2)+s[i]-'0';
  return ans;
}

inline void die(string code, string msg,bool sendKill=1) {
    if(sendKill)
    {
      for(int i=0;i<INSTANCES;i++)
      {
        fprintf(fout[i], "%s\n", input_secret.c_str());
        fprintf(fout[i], "%d\n", -1);
        fflush(fout[i]);
      }
    }
    if(code==OK)
    {
      for(int c=0;c<t;c++)
      {
        if(getnum(cur[c][0][0])!=ans[c])
        {
          code=WA;
        }
      }
    }
    //cerr << msg << endl;
    if(code==OK)
      cout << 1 << endl;
    else
      cout << 0 << endl;
    cerr << code << endl;
    //doubleWrite(code, msg);
    //finish();
    exit(0);
}


string getx(string a,string b)
{
  assert(a.size()==100);
  assert(b.size()==100);
  string ans="";
  for(int i=0;i<a.size();i++)
    if(a[i]!=b[i])
      ans+='1';
    else
      ans+='0';
  return ans;
}

struct ques
{
  int tc,i,j,k,n;
  vector<vector<string> > a;
  void ask(int inst)
  {
    fprintf(fout[inst], "%d %d %d %d\n",i^ix,j^jx,k^kx,n^nx);
    for(int x=0;x<3;x++)
      for(int y=0;y<3;y++)
      {
        fprintf(fout[inst], "%s%c",getx(a[x][y],A[x][y]).c_str()," \n"[y==2]);
      }
  }
  void readAns(int inst)
  {
      char x[105];
      if(fscanf(fin[inst],"%s",x)!=1) die(WA,"no in",1);
      //if(x<0||x>MXA) die(WA,"",1);
      cur[tc][i][j]=x;
      if(cur[tc][i][j].size()!=100) die(WA,"invalid len",1);
      for(int m=0;m<100;m++)
        if(cur[tc][i][j][m]!='0'&&cur[tc][i][j][m]!='1') die(WA,"invalid in",1);
  }
};

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
    mxk=max(mxk,k[c]);
    grid[c]=vector<string>(2*k[c]+1,string(2*k[c]+1,'.'));
    cur[c]=vector<vector<string> >(2*k[c]+1,vector<string>(2*k[c]+1,string(100,'0')));
    for(int x=0;x<2*k[c]+1;x++)
    {
      for(int y=0;y<2*k[c]+1;y++)
      {
        scanf(" %c",&grid[c][x][y]);
        cur[c][x][y][0]=grid[c][x][y];
      }
    }
    ans[c]=calcAns(c);
  }
}

void processPhase(int p)
{
  vector<ques> tobe[INSTANCES][t];
  vector<int> cnt(INSTANCES);
  for(int tc=0;tc<t;tc++)
  {
    for(int i=0;i<2*(k[tc]-p)-1;i++)
    {
      for(int j=0;j<2*(k[tc]-p)-1;j++)
      {
        int curin=getRand(0,INSTANCES-1);
        vector<vector<string> > v(3);
        for(int x=0;x<3;x++)
        {
          v[x]=vector<string>(3);
          for(int y=0;y<3;y++) 
            v[x][y]=cur[tc][i+x][j+y];
        }
        tobe[curin][tc].push_back({tc,i,j,p,k[tc],v});
        cnt[curin]++;
      }
    }
  }
  for(int in=0;in<INSTANCES;in++)
  {
    fprintf(fout[in], "%s\n",input_secret.c_str());
    fprintf(fout[in], "%d\n",cnt[in]);
    vector<int> curid(t,0);
    vector<int> rem;
    for(int i=0;i<t;i++)
      if(tobe[in][i].size()) rem.push_back(i);
    vector<pair<int,int> > ord;
    while(rem.size())
    {
      int x=getRand(0,(int)rem.size()-1);
      int tc=rem[x];
      tobe[in][tc][curid[tc]].ask(in);
      ord.push_back({tc,curid[tc]});
      curid[tc]++;
      if(curid[tc]==(int)tobe[in][tc].size())
      {
        swap(rem[x],rem.back());
        rem.pop_back();
      }
    }
    fflush(fout[in]);
    char secret[1000];
    if(1 != fscanf(fin[in],"%s", secret)) die(WA,"",1);
    if (string(secret) != output_secret) {
      die(SV,"",1);
    }
    for(int i=0;i<ord.size();i++)
    {
      int x=ord[i].first,y=ord[i].second;
      tobe[in][x][y].readAns(in);
    }
  }
}

int main(int argc, char **argv) {
  signal(SIGPIPE, SIG_IGN);
  
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
  for(int i=0;i<mxk;i++)
    processPhase(i);
  die(OK,"");
  return 0;
}
