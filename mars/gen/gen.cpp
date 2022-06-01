#include "bits/stdc++.h"
#include "testlib.h"
using namespace std;

template<int D, typename T>
struct Vec : public vector<Vec<D - 1, T>> {
    template<typename... Args>
    Vec(int n = 0, Args... args) : vector<Vec<D - 1, T>>(n, Vec<D - 1, T>(args...)) { }
};
template<typename T>
struct Vec<1, T> : public vector<T> {
    Vec(int n = 0, const T& val = T()) : vector<T>(n, val) { }
};

struct DSU{
    vector <int> e;
    DSU(int n) : e(n ,-1) {}
    int find(int x) { return e[x]<0? x : e[x]=find(e[x]); }
    int size(int x) { return -e[find(x)]; }
    bool join(int x ,int y){
        x = find(x) ,y = find(y);
        if(x == y) return false;
        if(e[x] > e[y]) swap(x ,y);
        e[x] += e[y]; e[y] = x;
        return true;
    }
};

const int di[] = {0, +1, 0, -1};
const int dj[] = {-1, 0, +1, 0};

using grid  = vector <string>;
grid new_grid(int n ,char b = '0'){
    return grid(n ,string(n ,b));
}
bool inside(int i ,int j ,int n){
    return 0 <= i && i < n && 0 <= j && j < n;
}
int to_scalar(int i ,int j ,int n){
    return n*i + j;
}
bool no_adj(int y ,int x ,int f ,grid&g){
    if(f != -1){
        f += 2;
        f %= 4;
    }
    if(!inside(y ,x ,g.size()) || g[y][x] == '1')
        return false;
    for(int d = 0; d < 4; d++) if(inside(y+di[d] ,x+dj[d] ,g.size()) && d != f)
        if(g[y+di[d]][x+dj[d]] == '1')
            return false;
    return true;
}
void write_grid(grid&a ,grid&b ,int y ,int x){
    assert(y-1+b.size() < a.size());
    assert(x-1+b.size() < a.size());
    for(int i = 0; i < b.size(); i++)
    for(int j = 0; j < b.size(); j++)
        a[y+i][x+j] = b[i][j];
}
void or_grid(grid&a ,grid&b){
    assert(a.size() == b.size());
    for(int i = 0; i < a.size(); i++)
    for(int j = 0; j < a.size(); j++) if(b[i][j] == '1')
        a[i][j] = '1';
}
void and_grid(grid&a ,grid&b){
    assert(a.size() == b.size());
    for(int i = 0; i < a.size(); i++)
    for(int j = 0; j < a.size(); j++) if(b[i][j] == '0')
        a[i][j] = '0';
}
void resize_grid(grid&a ,int n){
    while(a.size() > n)
        a.pop_back();
    while(a.size() < n)
        a.push_back(string(n ,'0'));
    for(int i = 0; i < n; i++)
        a[i].resize(n ,'0');
}
void rotate_grid(grid&a){
    int n = a.size();
    for(int i = 0; i < n/2; i++){
        for(int j = i; j < n-i-1; j++){
            int k = a[i][j];
            a[i][j] = a[n-1-j][i];
            a[n-1-j][i] = a[n-1-i][n-1-j];
            a[n-1-i][n-1-j] = a[j][n-1-i];
            a[j][n-1-i] = k;
        }
    }
}

grid concentric_squares(int n ,int x){
    grid g = new_grid(n);
    for(int d = 0; d <= n/2; d += x){
        for(int i = d; i < n-d; i++){
            g[i][d] = '1' ,g[i][n-d-1] = '1';
            g[d][i] = '1' ,g[n-d-1][i] = '1';
        }
    }
    return g;
}
grid concentric_squares_nocycles(int n ,int x){
    grid g = new_grid(n);
    for(int d = 0; d <= n/2; d += x){
        for(int i = d; i < n-d; i++){
            g[i][d] = '1' ,g[i][n-d-1] = '1';
            g[d][i] = '1' ,g[n-d-1][i] = '1';
        }
        if(d == n-d-1)
            continue;
        int i = rnd.next(d ,n-d-1);
        switch(rnd.next(4)){
            case 0: g[i][d] = '0'; break;
            case 1: g[d][i] = '0'; break;
            case 2: g[i][n-d-1] = '0'; break;
            case 3: g[n-d-1][i] = '0'; break;
        }
    }
    return g;
}
grid single_cells(int n ,int x){
    grid g = new_grid(n);
    for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++)
        g[i][j] = '0' + ((i+j)%2 == x);
    return g;
}
grid one_spiral(int n ,int x){
    grid g = new_grid(n);
    int i = (x>>2&1)*(n-1);
    int j = (x>>1&1)*(n-1);
    int d = (x>>0&1)*2;
    for(int s = 0; s < n*n; s++){
        g[i][j] = '1';
        if(inside(i+di[d] ,j+dj[d] ,n) && g[i+di[d]][j+dj[d]] == '0'
        &&(!inside(i+2*di[d] ,j+2*dj[d] ,n) || g[i+2*di[d]][j+2*dj[d]] == '0')){
            i += di[d];
            j += dj[d];
        }
        else{
            d += 1;
            d %= 4;
        }
    }
    return g;
}
grid random_cells(int n ,int p){
    grid g = new_grid(n);
    for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++) if(rnd.next(1 ,100) <= p)
        g[i][j] = '1';
    return g;
}
grid horizontal_stripes(int n ,bool x){
    grid g = new_grid(n);
    for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++) if(i%2 == x)
        g[i][j] = '1';
    return g;
}
grid vertical_stripes(int n ,bool x){
    grid g = new_grid(n);
    for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++) if(j%2 == x)
        g[i][j] = '1';
    return g;
}
grid diagonal_stairs(int n ,bool x){
    grid g = new_grid(n ,'1');
    for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++) if((i+j)%3 == 0)
        g[i][j] = '0';
    for(auto&r : g) if(x)
        reverse(r.begin() ,r.end());
    return g;
}
grid snake(int n ,bool x){
    grid g = new_grid(n);
    for(int i = 0; i < n; i += 2){
        for(int j = 0; j < n; j++)
            g[i][j] = '1';
        if(i+1 < n)
            g[i+1][(i>>1&1)*(n-1)] = '1';
    }
    if(x){
        for(int i = 0; i < n; i++)
        for(int j = 0; j < i; j++)
            swap(g[i][j] ,g[j][i]);
    }
    return g;
}
void dfs(int i ,int j ,int&n ,grid&g){
    g[i][j] = '1';
    vector <int> dir = {0 ,1 ,2 ,3};
    shuffle(dir.begin() ,dir.end());
    for(auto&d : dir) if(no_adj(i+di[d] ,j+dj[d] ,d ,g))
        dfs(i+di[d] ,j+dj[d] ,n ,g);
}
grid random_dfs(int n ,int r){
    grid g = new_grid(n);
    dfs(rnd.next(n) ,rnd.next(n) ,n ,g);
    vector <array <int ,2>> c;
    for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++) if(g[i][j] == '1')
        c.push_back({i ,j});
    shuffle(c.begin() ,c.end());
    assert(r < c.size());
    c.resize(r);
    for(auto&[i ,j] : c)
        g[i][j] = '0';
    return g;
}
grid center_bfs(int n ,int r){
    grid g = new_grid(n);
    vector <int> par(n*n ,-1) ,sz(n*n ,0);
    queue <array <int ,3>> q{{{n/2 ,n/2 ,0}}};
    g[q.front()[0]][q.front()[1]] = '1';
    while(!q.empty()){
        auto [i ,j ,dep] = q.front(); q.pop();
        for(int d = 0; d < 4; d++) if(no_adj(i+di[d] ,j+dj[d] ,d ,g)){
            g[i+di[d]][j+dj[d]] = '1';
            par[to_scalar(i+di[d] ,j+dj[d] ,n)] = to_scalar(i ,j ,n);
            for(int u = to_scalar(i+di[d] ,j+dj[d] ,n); ~u; u = par[u])
                sz[u]++;
            q.push({i+di[d] ,j+dj[d]});
        }
    }
    vector <int> ord(n*n);
    iota(ord.begin() ,ord.end() ,0);
    sort(ord.begin() ,ord.end() ,[&](int&i ,int&j){ return sz[i] > sz[j]; });
    int lsz = -1;
    vector <array <int ,2>> cells;
    for(int&s : ord){
        if(sz[s] != lsz){
            shuffle(cells.begin() ,cells.end());
            if(cells.size() > r)
                cells.resize(r);
            for(auto&[i ,j] : cells)
                g[i][j] = '0';
        }
        else
            cells.push_back({s/n ,s%n});
        lsz = sz[s];
    }
    return g;
}
grid random_square_shapes(int n ,int s){
    grid g = new_grid(n + (n%s == 0? 0 : s-n%s));
    for(int y = 0; y < g.size(); y += s)
    for(int x = 0; x < g.size(); x += s){
        grid r;
        switch(rnd.next(7)){
            case 0: r = new_grid(s-1 ,'1'); break;
            case 1: r = one_spiral(s-1 ,rnd.next(8)); break;
            case 2: r = snake(s-1 ,rnd.next(2)); break;
            case 3: r = horizontal_stripes(s-1 ,rnd.next(2)); break;
            case 4: r = vertical_stripes(s-1 ,rnd.next(2)); break;
            case 5: r = concentric_squares(s-1 ,2); break;
            case 6: r = diagonal_stairs(s-1 ,rnd.next(2)); break;
        }
        write_grid(g ,r ,y ,x);
    }
    resize_grid(g ,n);
    return g;
}
grid random_rectangles(int n ,int wl ,int wr ,int hl ,int hr){
    grid g = new_grid(n);
    vector <array <int ,4>> poss;
    for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++)
    for(int y = i+hl; y <= min(i+hr ,n); y++)
    for(int x = j+wl; x <= min(j+wr ,n); x++)
        poss.push_back({i ,j ,y ,x});
    while(!poss.empty()){
        auto [i ,j ,y ,x] = poss[rnd.next(poss.size())];
        for(int r = i; r < y; r++)
        for(int c = j; c < x; c++)
            g[r][c] = '1';
        auto isct = [&](int ti ,int tj ,int ty ,int tx){
            ti = max(ti ,i) ,ty = min(ty ,y);
            tj = max(tj ,j) ,tx = min(tx ,x);
            if(ti == ty && tj == tx){
                if(ti == i && tj == j) return false;
                if(ti == i && tj == x) return false;
                if(ti == y && tj == j) return false;
                if(ti == y && tj == x) return false;
                return true;
            }
            return ti <= ty && tj <= tx;
        };
        vector <array <int ,4>> newposs;
        for(auto&p : poss) if(!isct(p[0] ,p[1] ,p[2] ,p[3]))
            newposs.push_back(p);
        poss = newposs;
    }
    return g;
}
grid concentric_triangles(int n ,bool x){
    grid g = new_grid(n);
    for(int d = n-1 ,c = 0; d > 0; d -= 2 ,c += 2){
        for(int i = 0; i < n && d-i >= 0; i++)
            if(d-i >= c && i >= c)
                g[i][d-i] = '1';
        d--;
        bool ok = false;
        for(int i = 0; i < n && d-i >= 0; i++)
            if(d-i >= c && i >= c)
                g[i][d-i] = '1' ,ok = true;
        if(ok)
        for(int i = c; g[i][c] != '1'; i++)
            g[i][c] = '1' ,g[c][i] = '1';
    }
    auto r = g;
    rotate_grid(r);
    rotate_grid(r);
    or_grid(g ,r);
    if(x)
        rotate_grid(g);
    return g;
}
grid connected_square_regions(int n ,int s ,int p){
    grid g = new_grid(n + (n%s == 0? 0 : s-n%s) ,'1');
    for(int i = 0; i < g.size(); i++)
    for(int j = 0; j < g.size(); j++)
        if((i%s == s-1) || (j%s == s-1))
            g[i][j] = '0';
    int rws = (n+s-1) / s;
    vector <array <int ,2>> eds;
    for(int i = 0; i < rws; i++)
    for(int j = 0; j < rws; j++){
        if(i+1 < rws)
            eds.push_back({to_scalar(i ,j ,rws) ,to_scalar(i+1 ,j ,rws)});
        if(j+1 < rws)
            eds.push_back({to_scalar(i ,j ,rws) ,to_scalar(i ,j+1 ,rws)});
    }
    int rgs = rws*rws;
    DSU d(rgs);
    int spn = rgs - 1;
    spn *= p/100.0;
    while(spn--){
        auto&[u ,v] = eds[rnd.next(eds.size())];
        int r = v/rws * s;
        int c = v%rws * s;
        if(u/rws == v/rws){
            for(int i = r; i < r+s-1; i++)
                g[i][c-1] = '1';
        }
        else{
            for(int j = c; j < c+s-1; j++)
                g[r-1][j] = '1';
        }
        d.join(u ,v);
        vector <array <int ,2>> neweds;
        for(auto&e : eds)
            if(d.find(e[0]) != d.find(e[1]))
                neweds.push_back(e);
        eds = neweds;
    }
    resize_grid(g ,n);
    return g;
}
grid random_dsu(int n ,int r){
    grid g = connected_square_regions(n ,2 ,100);
    vector <array <int ,2>> c;
    for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++) if(g[i][j] == '1')
        c.push_back({i ,j});
    shuffle(c.begin() ,c.end());
    assert(r < c.size());
    c.resize(r);
    for(auto&[i ,j] : c)
        g[i][j] = '0';
    return g;
}
grid delete_lines(int n ,int l ,int p){
    grid g = random_cells(n ,p);
    while(l--){
        int y = rnd.next(n);
        int x = rnd.next(n);
        int t = rnd.next(4);
        if(t == 0){
            for(int i = 0; i < n; i++)
                g[i][x] = '0';
        }
        else if(t == 1){
            for(int j = 0; j < n; j++)
                g[y][j] = '0';
        }
        else if(t == 2){
            for(int k = -n; k <= n; k++)
                if(inside(y+k ,x+k ,n))
                    g[y+k][x+k] = '0';
        }
        else if(t == 3){
            for(int k = -n; k <= n; k++)
                if(inside(y+k ,x-k ,n))
                    g[y+k][x-k] = '0';
        }
    }
    return g;
}
grid delete_lines_onesea(int n ,int l ,int p){
    grid g = random_cells(n ,p);
    for(int i = 0; i < n; i++){
        g[i][0] = '0' ,g[0][i] = '0';
        g[i][n-1] = '0' ,g[n-1][i] = '0';
    }
    while(l--){
        int y = rnd.next(n);
        int x = rnd.next(n);
        int t = rnd.next(4);
        if(t == 0){
            for(int i = 0; i < n; i++)
                g[i][x] = '0';
        }
        else if(t == 1){
            for(int j = 0; j < n; j++)
                g[y][j] = '0';
        }
        else if(t == 2){
            for(int k = -n; k <= n; k++){
                if(inside(y+k ,x+k ,n))
                    g[y+k][x+k] = '0';
                if(inside(y+k+1 ,x+k ,n))
                    g[y+k+1][x+k] = '0';
            }
        }
        else if(t == 3){
            for(int k = -n; k <= n; k++){
                if(inside(y+k ,x-k ,n))
                    g[y+k][x-k] = '0';
                if(inside(y+k+1 ,x-k ,n))
                    g[y+k+1][x-k] = '0';
            }
        }
    }
    return g;
}
grid random_disjoint_walks(int n ,int l ,int p){
    grid g = new_grid(n);
    vector <array <int ,2>> poss;
    for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++)
        poss.push_back({i ,j});
    while(p-- && !poss.empty()){
        auto&[i ,j] = poss[rnd.next(poss.size())];
        for(int r = 0; r < l; r++){
            g[i][j] = '1';
            vector <int> ord(4);
            iota(ord.begin() ,ord.end() ,0);
            shuffle(ord.begin() ,ord.end());
            for(int&d : ord) if(no_adj(i+di[d] ,j+dj[d] ,d ,g)){
                i += di[d];
                j += dj[d];
                break;
            }
        }
        vector <array <int ,2>> newposs;
        for(auto&[i ,j] : poss) if(no_adj(i ,j ,-1 ,g) && g[i][j] == '0')
            newposs.push_back({i ,j});
        poss = newposs;
    }
    return g;
}
grid convert_to_onesea(grid&g){
    int n = g.size();
    g[0][0] = '0';
    Vec <2 ,int> rgs(n ,n ,-1);
    auto color = [&](int y ,int x ,int c){
        if(g[y][x] == '1' || ~rgs[y][x])
            return 0;
        rgs[y][x] = c;
        queue <array <int ,2>> q{{{y ,x}}};
        while(!q.empty()){
            auto [i ,j] = q.front(); q.pop();
            for(int d = 0; d < 4; d++){
                if(inside(i+di[d] ,j+dj[d] ,n))
                if(g[i+di[d]][j+dj[d]] == '0')
                if(rgs[i+di[d]][j+dj[d]] != c){
                    q.push({i+di[d] ,j+dj[d]});
                    rgs[i+di[d]][j+dj[d]] = c;
                }
            }
        }
        return 1;
    };
    vector <array<int ,2>> gridord;
    for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++)
        gridord.push_back({i ,j});
    while(true){
        for(int i = 0; i < n; i++)
            rgs[i].assign(n ,-1);
        int diff = 0;
        for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            diff += color(i ,j ,diff);
        if(diff <= 1) break;
        vector <bool> pro(diff);
        vector <array <int ,2>> shpath;
        shuffle(gridord.begin() ,gridord.end());
        for(auto [i ,j] : gridord) if(~rgs[i][j] && !pro[rgs[i][j]]){
            pro[rgs[i][j]] = true;
            Vec <2 ,array<int ,2>> from(n ,n ,array<int ,2>{-1 ,-1});
            deque <array <int ,2>> q{{{i ,j}}};
            while(!q.empty()){
                auto [y ,x] = q.front(); q.pop_front();
                if(~rgs[y][x] && rgs[y][x] != rgs[i][j]){
                    vector <array <int ,2>> path{{y ,x}};
                    while(rgs[path.back()[0]][path.back()[1]] != rgs[i][j]){
                        path.push_back(from[path.back()[0]][path.back()[1]]);
                    }
                    if(shpath.empty() || shpath.size() > path.size())
                        shpath = path;
                    break;
                }
                vector <int> ord(4);
                iota(ord.begin() ,ord.end() ,0);
                shuffle(ord.begin() ,ord.end());
                for(int&d : ord){
                    if(inside(y+di[d] ,x+dj[d] ,n) && !~from[y+di[d]][x+dj[d]][0]){
                        from[y+di[d]][x+dj[d]] = {y ,x};
                        if(g[y+di[d]][x+dj[d]] == '0')
                            q.push_front({y+di[d] ,x+dj[d]});
                        else
                            q.push_back({y+di[d] ,x+dj[d]});
                    }
                }
            }
        }
        for(auto&[i ,j] : shpath)
            g[i][j] = '0';
    }
    return g;
}

int main(int argc, char* argv[])
{
#ifdef LOCAL
    ifstream argfile("arg.txt");
    string line ,token;
    vector <string> varg;
    while(getline(argfile ,line)){
        istringstream iss(line);
        while(iss >> token)
            varg.push_back(token);
        if(varg.front() == ">")
            break;
        varg.clear();
    }
    varg[0] = string(argv[0]);
    argfile.close();
    char*argvz[varg.size()];
    argv = argvz;
    for(int i = 0; i < varg.size(); i++)
        argv[i] = &varg[i][0];
    argc = varg.size();
    freopen("test.in" ,"w" ,stdout);
#endif LOCAL

    registerGen(argc ,argv ,1);
    string func(argv[1]);
    int k = atoi(argv[2]);

    grid g;
    if(func == "concentric_squares"){
        int d = atoi(argv[3]);
        g = concentric_squares(2*k+1 ,d);
    }
    else if(func == "single_cells"){
        int d = atoi(argv[3]);
        g = single_cells(2*k+1 ,d);
    }
    else if(func == "one_spiral"){
        int d = atoi(argv[3]);
        g = one_spiral(2*k+1 ,d);
    }
    else if(func == "random_cells"){
        int d = atoi(argv[3]);
        g = random_cells(2*k+1 ,d);
    }
    else if(func == "horizontal_stripes"){
        int d = atoi(argv[3]);
        g = horizontal_stripes(2*k+1 ,d);
    }
    else if(func == "vertical_stripes"){
        int d = atoi(argv[3]);
        g = vertical_stripes(2*k+1 ,d);
    }
    else if(func == "diagonal_stairs"){
        int d = atoi(argv[3]);
        g = diagonal_stairs(2*k+1 ,d);
    }
    else if(func == "random_dfs"){
        int d = atoi(argv[3]);
        g = random_dfs(2*k+1 ,d);
    }
    else if(func == "center_bfs"){
        int d = atoi(argv[3]);
        g = center_bfs(2*k+1 ,d);
    }
    else if(func == "snake"){
        int d = atoi(argv[3]);
        g = snake(2*k+1 ,d);
    }
    else if(func == "random_square_shapes"){
        int d = atoi(argv[3]);
        g = random_square_shapes(2*k+1 ,d);
    }
    else if(func == "random_rectangles"){
        int wl = atoi(argv[3]);
        int wr = atoi(argv[4]);
        int hl = atoi(argv[5]);
        int hr = atoi(argv[6]);
        g = random_rectangles(2*k+1 ,wl ,wr ,hl ,hr);
    }
    else if(func == "concentric_triangles"){
        int d = atoi(argv[3]);
        g = concentric_triangles(2*k+1 ,d);
    }
    else if(func == "connected_square_regions"){
        int s = atoi(argv[3]);
        int p = atoi(argv[4]);
        g = connected_square_regions(2*k+1 ,s ,p);
    }
    else if(func == "random_dsu"){
        int d = atoi(argv[3]);
        g = random_dsu(2*k+1 ,d);
    }
    else if(func == "delete_lines"){
        int l = atoi(argv[3]);
        int p = atoi(argv[4]);
        g = delete_lines(2*k+1 ,l ,p);
    }
    else if(func == "random_disjoint_walks"){
        int l = atoi(argv[3]);
        int p = atoi(argv[4]);
        g = random_disjoint_walks(2*k+1 ,l ,p);
    }
    else if(func == "concentric_squares_nocycles"){
        int d = atoi(argv[3]);
        g = concentric_squares_nocycles(2*k+1 ,d);
    }
    else if(func == "delete_lines_onesea"){
        int l = atoi(argv[3]);
        int p = atoi(argv[4]);
        g = delete_lines_onesea(2*k+1 ,l ,p);
    }
    if(string(argv[argc-1]) == "one_sea")
        g = convert_to_onesea(g);

#ifdef LOCAL
    printf("%d\n",g.size());
    for(auto&r : g)
        printf("%s\n",r.c_str());
#endif LOCAL
#ifndef LOCAL
    printf("%d\n",(int)g.size()/2);
    for(int i = 0; i < g.size(); i++)
    for(int j = 0; j < g.size(); j++)
        printf("%c%c",g[i][j]," \n"[j+1 == g.size()]);
#endif
}
