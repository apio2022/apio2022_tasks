#include "mars.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cassert>
#include <csignal>


using namespace std;

/*static FILE *fin;
static FILE *fout;*/

static string input_secret="224bb858-b13b-5e97-cbba-4a10b0455e79";
static string output_secret="f18fba32-f6de-4dd0-ef1b-ea027937a4aa";
static string sv_secret="dcc49c2d-df9d-dc50-3ba2-ef7527b9539b";
static int ix=935,jx=391,kx=626,nx=426;
static string A[3][3]={{"1010111000011100001101110100011101000000010110011011101010001010000001011000010011000001111011111111", "1101011000101100110110100011110010000010000100001010001110101111010000100001110000001110110011001010", "1110111001011000110000110000011011100110001101000110011000100110010110010000010100010010001000010011"},{"0001000101000011100000000101101111010000110011101101001100101100011111010011011001011000111010010110", "0101110011100100000011001001111100110110001011111011001000111100000000010000100110100001010010010000", "0011011100110001001101010011100011100000100110110011110001011100110010110111100001111000100101111011"},{"0100110010100000000000011101110110000010101011111010111011100000110000010101000111100100001010001000", "1000111110010110100111001100000010011010000011001111001100110000010001111011111100101000010100101111", "0001001111100010010011101111110000010101101100011110101100000111100100101111000001001111011110000100"}};

static string getx(string a,string b)
{
  string ans="";
  assert(a.size()==100&&b.size()==100);
  for(int i=0;i<100;i++)
    if(a[i]!=b[i])
      ans+='1';
    else
      ans+='0';
  return ans;
}

int main() {
    /*if(argc<3) {
        cerr << "grader error" << endl;
        cerr << "number of argument isn't 3" << endl;
        exit(0);
    }
    fin = fopen(argv[1], "r11");
    fout = fopen(argv[2], "a");*/
    signal(SIGPIPE, SIG_IGN);

    while(true)
    {
        char secret[1000];
        if(1 != scanf("%s", secret)){
            cout << sv_secret << endl;
            exit(0);
        }
        if (string(secret) != input_secret) {
            cout << sv_secret << endl;
            exit(0);
        }
        int sz;
        if(scanf("%d",&sz)!=1){
            cout << sv_secret << endl;
            exit(0);
        }
        if(sz==-1) exit(0);
        vector<string> res;
        for(int x=0;x<sz;x++)
        {
            int i,j,k,n;
            if(scanf("%d %d %d %d",&i,&j,&k,&n)!=4){
                cout << sv_secret << endl;
                exit(0);
            }
            i^=ix; j^=jx; k^=kx; n^=nx; 
            vector<vector<string> > v(3,vector<string>(3));
            for(int x=0;x<3;x++)
            {
                for(int y=0;y<3;y++)
                {
                    char tmp[105];
                    if(scanf("%s",tmp)!=1){
                       cout << sv_secret << endl;
                        exit(0);
                    }
                    v[x][y]=getx(A[x][y],string(tmp));
                }
            }
            res.push_back(process(v,i,j,k,n));
        }
        printf("%s\n",output_secret.c_str());
        for(int i=0;i<(int)res.size();i++)
            printf("%s\n",res[i].c_str());
        fflush(stdout);
    }
    return 0;
}
