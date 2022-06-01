#include "bits/stdc++.h"
#include "mars.h"
using namespace std;

const int W = 100;
const int B = 9;
using word = bitset <W>;

struct DSU{
    vector <int> e;
    DSU(int n) : e(n ,-1) {}
    int find(int x) { return e[x] < 0? x : e[x] = find(e[x]); }
    void join(int x ,int y){
        x = find(x) ,y = find(y);
        if(x && y && x != y)
            e[x] += e[y] ,e[y] = x;
    }
};

word encode(vector <int> e){
    int l = e.size();
    e.resize(l+2);
    vector <int> fst(l+1 ,-1) ,lst(l+1 ,-1);
    for(int i = 0; i < e.size(); i++) if(e[i]){
        fst[e[i]] = ~fst[e[i]]? fst[e[i]] : i;
        lst[e[i]] = i;
    }
    for(int i = 0; i < e.size(); i++) if(e[i]){
        if(fst[e[i]] == lst[e[i]]) e[i] = 1;
        else if(i == fst[e[i]])    e[i] = 2;
        else if(i == lst[e[i]])    e[i] = 3;
        else                       e[i] = 4;
    }
    word w;
    for(int i = 0; i+2 < e.size(); i += 3){
        int m = e[i] + e[i+1]*5 + e[i+2]*25;
        for(int j = 0; j < 7; j++)
            w[7*i/3+j] = m>>j&1;
    }
    return w;
}
vector <int> decode(word w ,int l){
    if(l == 1) return {w.any()};
    vector <int> e(l+2);
    for(int i = 0; i+2 < e.size(); i += 3){
        int m = 0;
        for(int j = 0; j < 7; j++)
            m |= w[7*i/3+j]<<j;
        e[i] = m%5;
        e[i+1] = (m/5)%5;
        e[i+2] = (m/25)%5;
    }
    vector <int> b;
    for(int id = 0 ,i = 0; i < e.size(); i++) if(e[i]){
        if(e[i] == 1)      { e[i] = ++id; }
        else if(e[i] == 2) { e[i] = ++id; b.push_back(id); }
        else if(e[i] == 3) { e[i] = b.back(); b.pop_back(); }
        else if(e[i] == 4) { e[i] = b.back(); }
    }
    e.resize(l);
    return e;
}

int diagLen(int i ,int j ,int n){
    return n - abs(i - j);
}
int diagOrd(int i ,int j ,int n){
    return diagLen(i ,j ,n) - min(i ,j) - 1;
}

pair <int ,vector <int>> buffer(vector<vector<word>> a ,int i ,int j ,int n){
    DSU d(3*n);
    vector <word> diag = {a[0][0]|a[1][1]|a[2][2] ,a[1][0]|a[2][1]};
    vector <int> diagConn[3] = {{} ,{} ,decode(a[2][0] ,diagLen(i+2 ,j ,n))};
    for(int id = n ,r = 1; r >= 0; r--){
        for(int k = 0; k < diagLen(i+r ,j ,n); k++){
            diagConn[r].push_back(diag[r][diagOrd(i+r+k ,j+k ,n)]? ++id : 0);
            if(k < diagConn[r+1].size())
                d.join(diagConn[r][k] ,diagConn[r+1][k]);
            if(k > 0)
                d.join(diagConn[r][k] ,diagConn[r+1][k-1]);
        }
    }

    int compsCnt = (a[2][0] >> (W-B)).to_ulong();
    vector <int> mp(3*n ,-1); mp[0] = 0;
    for(int id = 0 ,r = 0; r < 3; r++){
        for(int&k : diagConn[r]){
            k = d.find(k);
            compsCnt += (r && mp[k] == -1);
            k = ~mp[k]? mp[k] : mp[k] = ++id;
        }
    }

    return {compsCnt ,diagConn[0]};
}

string process(vector<vector<string>> a ,int i ,int j ,int k ,int n)
{
    vector<vector<word>> b(3 ,vector<word>(3));
    for(int y = 0; y < 3; y++)
    for(int x = 0; x < 3; x++)
    {
        reverse(a[y][x].begin(),a[y][x].end());
        b[y][x] = word(a[y][x]);
    }

    n = 2*n+1 ,k = 2*k+3;
    auto transpose = [&](){
        swap(i ,j);
        swap(b[0][1] ,b[1][0]);
        swap(b[0][2] ,b[2][0]);
        swap(b[1][2] ,b[2][1]);
    };
    if(j == n-k && i == 0)
        transpose();

    if(k == 3){ //first phase
        for(int y = 0; y < 3; y++)
        for(int x = 0; x < 3; x++)
            b[y][x] <<= diagOrd(i+y ,j+x ,n);
    }
    if(k != n){ //not last phase
        b[0][0] |= b[1][1] | b[2][2];
    }

    if(k == n){ //last phase
        auto [cnt1 ,conn1] = buffer(b ,i ,j ,n);
        transpose();
        auto [cnt2 ,conn2] = buffer(b ,i ,j ,n);

        DSU d(n+1);
        for(auto&conn : {conn1 ,conn2}){
            vector <int> lst(n+1 ,-1);
            for(int l = 0; l < n; l++) if(conn[l]){
                if(~lst[conn[l]])
                    d.join(lst[conn[l]] ,l+1);
                lst[conn[l]] = l+1;
            }
        }

        cnt1 += cnt2;
        for(int l = 0; l < n; l++) if(conn1[l])
            cnt1 += l+1 == d.find(l+1);
        b[0][0] = word(cnt1);
    }
    else if(i == n-k && j == 0){ //first star
        auto [cnt ,conn] = buffer(b ,i ,j ,n);
        b[0][0] = (word(cnt) << (W-B)) | encode(conn);
    }
    string ret=b[0][0].to_string();
    reverse(ret.begin(),ret.end());
    return ret;
}
