#include "xrand1.h"
#include "testlib.h"
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <cstring>
#include <map>
#include <queue>
#include <iostream>
#define SIZE 500050

using namespace std;
typedef pair <int,int> P;
typedef pair <P,int> PP;

#define cerr (cerr<<"-- line "<<__LINE__<<" -- ")

template<class T,class U>
ostream& operator<<(ostream& os,const pair<T,U>& p){
	os<<"("<<p.first<<","<<p.second<<")";
	return os;
}

template<class T>
ostream& operator <<(ostream& os,const vector<T>& v){
	os<<"{";
	for(int i=0;i<(int)v.size();i++){
		if(i)os<<",";
		os<<v[i];
	}
	os<<"}";
	return os;
}

struct solver
{
	vector <int> vec[SIZE];
	vector <int> rvec[SIZE];
	int left[SIZE],right[SIZE];
	queue <int> que;
	int mum;
	
	void init(int n, int k)
	{
		mum=1;
		while(mum<k+2) mum<<=1;
		for(int i=0;i<k;i++) left[i]=i+1,right[i]=i+2;
		for(int i=k;i<n;i++) left[i]=0,right[i]=mum;
		for(int i=0;i<n;i++)
		{
			vec[i].clear();
			rvec[i].clear();
		}
		while(!que.empty()) que.pop();
	}
	bool check(int u,int v)
	{
		if(right[v]<=left[u]) return true;
		if((left[v]+right[v])/2<=left[u])
		{
			left[v]=(left[v]+right[v])/2;
			que.push(v);
		}
		if(right[v]<=(left[u]+right[u])/2)
		{
			right[u]=(left[u]+right[u])/2;
			que.push(u);
		}
		return false;
	}
	bool add_portal(int u, int v)
	{
		vec[u].push_back(v);
		rvec[v].push_back(u);
		if(check(u,v)) return true;
		while(!que.empty())
		{
			int x=que.front();que.pop();
			if(left[x]+1==right[x]) return true;
			for(int i=0;i<int(vec[x].size());i++)
			{
				int to=vec[x][i];
				if(check(x,to)) return true;
			}
			for(int i=0;i<int(rvec[x].size());i++)
			{
				int to=rvec[x][i];
				if(check(to,x)) return true;
			}
		}
		return false;
	}
}sol;
string taskName;
int TaskNumber;
map <string,int> num;
const int MAX_N=300000;
const int MAX_M=500000;
int col[SIZE];
XRand RND;

string numToStr(int x){
	string ret="";
	ret+=char('0'+(x/10));
	ret+=char('0'+(x%10));
	return ret;
}
void SetName()
{
	string name=taskName;
	name+="-";
	name+=numToStr(++num[name]);
	cerr<<name<<endl;
	#ifdef MAROON_LOCAL
	//do nothing
	#else
	freopen((string("manual/")+name+".txt").c_str(),"w",stdout);
	#endif
}
void Output(int N,int M,int K,vector <P> E,bool sh=true)
{
	ensure((int) E.size()==M);
	ensure(K<=N);
	ensure(N<=MAX_N);
	ensure(M<=MAX_M);
	if(TaskNumber==1) ensure(N==K);
	if(TaskNumber==1||TaskNumber==2)
	{
		ensure(N<=1000);
		ensure(M<=5000);
	}
	if(TaskNumber==3)
	{
		ensure(N<=30000);
		ensure(M<=50000);
		ensure(K<=1000);
	}
	if(sh){
		vector <int> ord;
		for(int i=K;i<N;i++) ord.push_back(i);
		RND.Shuffle(ord.begin(),ord.end());
		for(int i=0;i<int(E.size());i++)
		{
			P&p=E[i];
			ensure(p.first<N);
			ensure(p.second<N);
			if(p.first>=K) p.first=ord[p.first-K];
			if(p.second>=K) p.second=ord[p.second-K];
		}
	}
	set <P> es;
	for(int i=0;i+1<K;i++) es.insert(P(i,i+1));
	for(int i=0;i<int(E.size());i++)
	{
		P p=E[i];
		ensure(es.count(p)==0);
		es.insert(p);
	}
	sol.init(N,K);
	for(int i=0;i+1<int(E.size());i++)
	{
		ensure(!sol.add_portal(E[i].first,E[i].second));
	}
	ensure(sol.add_portal(E[E.size()-1].first,E[E.size()-1].second));
	
	SetName();
	printf("%d %d %d\n",N,M,K);
	for(int i=0;i<M;i++) printf("%d %d\n",E[i].first,E[i].second);
}
struct TestCase
{
	int N,K;
	vector<P> E;
};
template <class T>
void KnuthShuffle(vector<T>& arr,int b=0,int e=-1)
{
	if(e==-1)
		e=arr.size();
	for(int i=b;i<e;i++)
		swap(arr[RND.NextInt(b,i)],arr[i]);
}
vector<int> GetPerm(int N)
{
	vector<int> perm(N);
	for(int i=0;i<N;i++)
		perm[i]=i;
	KnuthShuffle(perm);
	return perm;
}
template<class T>
void ShuffleMerge(vector<T>& buf,int a,int b,int c)
{
	int p=b-a,q=c-b;
	vector<int> perm=GetPerm(p+q);
	vector<T> tmp(p+q);
	sort(perm.begin(),perm.begin()+p);
	sort(perm.begin()+p,perm.end());
	for(int i=a;i<c;i++)
		tmp[perm[i-a]]=buf[i];
	for(int i=a;i<c;i++)
		buf[i]=tmp[i-a];
}
void OutputTestCase(const TestCase& tc)
{
	Output(tc.N,tc.E.size(),tc.K,tc.E);
}
TestCase CombineTwoTest(const TestCase& a,const TestCase& b,bool sh=false)
{
	int N=a.N+b.N;
	int K=a.K+b.K;
	vector<int> kPerm=GetPerm(K);
	sort(kPerm.begin(),kPerm.begin()+a.K);
	sort(kPerm.begin()+a.K,kPerm.end());
	vector<P> E;
	{
		const auto Idx=[&](int i){
			if(i<a.K)
				return kPerm[i];
			else
				return K+(i-a.K);
		};
		for(auto e=a.E.begin();e+1!=a.E.end();e++)
			E.push_back(P(Idx(e->first),Idx(e->second)));
	}
	{
		const auto Idx=[&](int i){
			if(i<b.K)
				return kPerm[a.K+i];
			else
				return K+(a.N-a.K)+(i-b.K);
		};
		for(auto e:b.E)
			E.push_back(P(Idx(e.first),Idx(e.second)));
		if(sh)
			KnuthShuffle(E,0,int(E.size())-1);
	}
	
	return TestCase{N,K,E};
}
TestCase Rand_Sub(int N,int K,int M=500000)
{
	while(1){
		set <P> es;
		for(int i=0;i<K-1;i++) es.insert(P(i,i+1));
		vector <P> edge;
		sol.init(N,K);
		int t;
		for(t=0;t<M;t++)
		{
			int u = RND.NextUInt(N), v = RND.NextUInt(N);
			while(u == v || es.find(P(u,v)) != es.end() || max(u,v) < K) u = RND.NextUInt(N), v = RND.NextUInt(N);
			es.insert(P(u,v));
			edge.push_back(P(u,v));
			if(sol.add_portal(u, v))
				break;
		}
		if(t<M)
			return TestCase{N,K,edge};
	}
	assert(false);
}
void Rand(int N,int K,int M)
{
	OutputTestCase(Rand_Sub(N,K,M));
}
TestCase Rand_DAG_A_Sub(int N,int K,int M,int T,bool manyUpd)
{
	ensure(K>=2);
	ensure(M>=4);
	//int a=(M-1)/3,b=a,c=M-1-a-b;
	ensure(M>=1+1+K+K);
	int a=M-1-K-K,b=K,c=K;
	vector<P> edge;
	set<P> es;
	vector<vector<int>> g(N);
	const auto AddEdge=[&](int u,int v){
		edge.push_back(P(u,v));
		es.insert(P(u,v));
		g[u].push_back(v);
	};
	for(int i=K;i<N-1;i++)
		if(RND.NextUInt(T))
			AddEdge(i,i+1);
	ensure(int(es.size())<=a);
	while(int(es.size())<a){
		int u,v;
		do{
			u=RND.NextInt(K,N-1);
			v=RND.NextInt(K,N-1);
			if(u>v)swap(u,v);
		}while(u==v||es.count(P(u,v)));
		AddEdge(u,v);
	}
	while(int(es.size())<a+b){
		int u,v;
		do{
			u=RND.NextInt(0,K-2);
			v=RND.NextInt(K,N-1);
		}while(u==v||es.count(P(u,v)));
		AddEdge(u,v);
	}
	vector<int> reach(N,-1);
	for(int i=0;i<N;i++){
		if(i<K)reach[i]=i;
		for(auto to:g[i])
			reach[to]=max(reach[to],reach[i]);
	}
	while(int(es.size())<a+b+c){
		int u,v;
		do{
			u=RND.NextInt(K,N-1);
			v=RND.NextInt(reach[u]+1,K-1);
		}while(u==v||es.count(P(u,v)));
		AddEdge(u,v);
	}
	KnuthShuffle(edge);
	if(manyUpd){
		const auto Weight=[&](P p){
			if(K<=min(p.first,p.second))
				return -1;
			else if(p.first<K){
				return p.first;
			}else if(p.second<K){
				return K-1-p.second;
			}else
				assert(false);
		};
		sort(edge.begin(),edge.end(),[&](P p,P q){
			return Weight(p)<Weight(q);
		});
	}
	while(int(es.size())<M){
		int u,v;
		do{
			do{
				u=RND.NextInt(K,N-1);
			}while(reach[u]==-1);
			v=RND.NextInt(0,reach[u]);
		}while(u==v||es.count(P(u,v)));
		AddEdge(u,v);
	}
	return TestCase{N,K,edge};
}
void Rand_DAG_A(int N,int K,int M,int T,bool manyUpd)
{
	OutputTestCase(Rand_DAG_A_Sub(N,K,M,T,manyUpd));
}
TestCase Rand_DAG_B_Sub(int N,int K,int M,int T,bool manyUpd)
{
	ensure(K>=2);
	ensure(M>=4);
	//int a=(M-1)/3,b=a,c=M-1-a-b;
	ensure(M>=1+1+K+K);
	int a=(N-K)-1,b=(M-1-a)/2,c=(M-1-a)-b;
	vector<P> edge;
	set<P> es;
	vector<vector<int>> g(N);
	const auto AddEdge=[&](int u,int v){
		edge.push_back(P(u,v));
		es.insert(P(u,v));
		g[u].push_back(v);
	};
	for(int i=K+1;i<N;i++){
		int mx=-1;
		for(int j=0;j<T;j++)
			mx=max(mx,RND.NextInt(K,i-1));
		AddEdge(mx,i);
	}
	ensure(int(es.size())==a);
	while(int(es.size())<a+b){
		int u,v;
		do{
			u=RND.NextInt(0,K-2);
			v=RND.NextInt(K,N-1);
		}while(u==v||es.count(P(u,v)));
		AddEdge(u,v);
	}
	vector<int> reach(N,-1);
	for(int i=0;i<N;i++){
		if(i<K)reach[i]=i;
		for(auto to:g[i])
			reach[to]=max(reach[to],reach[i]);
	}
	//while(int(es.size())<a+b+c){
	for(int i=0;i<10000000&&int(es.size())<a+b+c;i++){
		int u,v;
		//do{
			u=RND.NextInt(K,N-1);
			v=RND.NextInt(reach[u]+1,K-1);
		//}while(u==v||es.count(P(u,v)));
		if(u==v||es.count(P(u,v)))continue;
		AddEdge(u,v);
	}
	KnuthShuffle(edge);
	if(manyUpd){
		const auto Weight=[&](P p){
			if(K<=min(p.first,p.second))
				return -1;
			else if(p.first<K){
				return p.first;
			}else if(p.second<K){
				return K-1-p.second;
			}else
				assert(false);
		};
		sort(edge.begin(),edge.end(),[&](P p,P q){
			return Weight(p)<Weight(q);
		});
	}
	//while(int(es.size())<M){
	{
		int u,v;
		do{
			do{
				u=RND.NextInt(K,N-1);
			}while(reach[u]==-1);
			v=RND.NextInt(0,reach[u]);
		}while(u==v||es.count(P(u,v)));
		AddEdge(u,v);
	}
	return TestCase{N,K,edge};
}
void Rand_DAG_B(int N,int K,int M,int T,bool manyUpd)
{
	OutputTestCase(Rand_DAG_B_Sub(N,K,M,T,manyUpd));
}
void All_Path(int N,int M,bool backedge)
{
	set <P> es;
	for(int i=0;i<N-1;i++) es.insert(P(i,i+1));
	vector <P> edge;
	for(int z = 0; z < 4 * M && int(edge.size()) < M - 1; z++) {
		int u = RND.NextUInt(N), v = RND.NextUInt(N);
		if(u > v) swap(u, v);
		if(u != v && es.find({u, v}) == es.end())
			es.insert(P(u,v)), edge.push_back(P(u,v));
	}
	int u = RND.NextUInt(N), v = RND.NextUInt(N);
	if(backedge) u = v;
	else if(u < v) swap(u, v);
	edge.push_back(P(u,v));
	Output(N,edge.size(),N,edge);
}
TestCase Two_Path_A_Sub(int N,int K)
{
	ensure(K<N);
	vector <P> edge;
	for(int i=K;i+1<N;i++)
	{
		edge.push_back(P(i,i+1));
	}
	KnuthShuffle(edge);
	int l=0,r=K-1;
	while(l<=r)
	{
		if(l==r)
		{
			if(RND.NextUInt(2))
			{
				edge.push_back(P(l,K));
				edge.push_back(P(N-1,r));
			}
			else
			{
				edge.push_back(P(N-1,r));
				edge.push_back(P(l,K));
			}
			break;
		}
		if(RND.NextInt(0,1))
		{
			edge.push_back(P(l,K));
			l++;
		}
		else
		{
			edge.push_back(P(N-1,r));
			r--;
		}
	}
	return TestCase{N,K,edge};
}
void Two_Path_A(int N,int K)
{
	OutputTestCase(Two_Path_A_Sub(N,K));
}
TestCase Two_Path_B_Sub(int N,int K,int M)
{
	ensure(K<N);
	vector <P> edge;
	for(int i=K;i+1<N;i++)
	{
		edge.push_back(P(i,i+1));
	}
	KnuthShuffle(edge);
	int W=N-K,B=W/3;
	ensure(B>0);
	int a=K,b=a+B,c=b+B,d=N;
	int l=0,r=K-1;
	const auto AddRemainEdge=[&](){
		set<P> es(edge.begin(),edge.end());
		int w=es.size();
		//while(int(es.size())<M-1){
		for(int i=0;i<10000000&&int(es.size())<M-1;i++){
			int u,v;
			if(RND.NextUInt(2))
				u=RND.NextInt(a,b-1),v=RND.NextInt(0,K-1);
			else
				u=RND.NextInt(0,K-1),v=RND.NextInt(c,d-1);
			if(es.count(P(u,v))==0){
				es.insert(P(u,v));
				edge.push_back(P(u,v));
			}
		}
		ShuffleMerge(edge,0,w,es.size());
	};
	while(l<=r)
	{
		if(l==r)
		{
			if(RND.NextUInt(2))
			{
				edge.push_back(P(l,b));
				AddRemainEdge();
				edge.push_back(P(c-1,r));
			}
			else
			{
				edge.push_back(P(c-1,r));
				AddRemainEdge();
				edge.push_back(P(l,b));
			}
			break;
		}
		if(RND.NextInt(0,1))
		{
			edge.push_back(P(l,b));
			l++;
		}
		else
		{
			edge.push_back(P(c-1,r));
			r--;
		}
	}
	return TestCase{N,K,edge};
}
void Two_Path_B(int N,int K,int M)
{
	OutputTestCase(Two_Path_B_Sub(N,K,M));
}
void Many_Path(int N,int K)
{
	ensure(K<N);
	vector <int> nd;
	nd.push_back(K);
	for(int t=K+1;t<=N;t++)
	{
		int r=RND.NextUInt((int) nd.size());
		if(r==0) nd.push_back(1);
		else nd[r]++;
	}
	vector<int> bg(nd.size()),ed(nd.size());
	bg[0]=0,ed[0]=K;
	vector <P> edge;
	int now=K;
	for(int i=1;i<int(nd.size());i++)
	{
		bg[i]=now;
		for(int j=0;j+1<nd[i];j++)
		{
			edge.push_back(P(now,now+1));
			now++;
		}
		now++;
		ed[i]=now;
	}
	for(int i=0;i<int(nd.size());i++)
	{
		int j=(i+1)%nd.size();
		edge.push_back(P(RND.NextInt(bg[i],ed[i]-1),bg[j]));
	}
	swap(edge[RND.NextInt(edge.size()-nd.size(),edge.size()-1)],edge.back());
	KnuthShuffle(edge,0,int(edge.size())-1);
	Output(N,edge.size(),K,edge);
}
vector <int> V_Make(int K,int sum,int x=0)
{
	sum-=x*K;
	ensure(K>0&&sum>=0);
	vector <int> ret;
	ret.push_back(sum);
	for(int i=1;i<K;i++) ret.push_back(RND.NextInt(0,sum));
	sort(ret.begin(),ret.end());
	for(int i=K-1;i>0;i--) ret[i]-=ret[i-1];
	for(int i=0;i<K;i++) ret[i]+=x;
	return ret;
}
void Cycle(int N,int K,int B,int M=500000)
{
	vector<int> ss(K,1);
	{
		int cur=K;
		while(cur<N){
			int s=RND.NextInt(1,min(B,N-cur));
			cur+=s;
			ss.push_back(s);
		}
	}
	vector<int> bg(ss.size()),ed(ss.size());
	vector<P> edge;
	{
		int cur=0;
		for(int i=0;i<int(ss.size());i++){
			bg[i]=cur;
			ed[i]=(cur+=ss[i]);
			if(K<=i)
				for(int j=0;j<ss[i];j++)
					edge.push_back(P(bg[i]+j,bg[i]+((j+1)%ss[i])));
		}
	}
	KnuthShuffle(edge);
	TestCase tc=Rand_Sub(ss.size(),K,M-edge.size());
	const auto Idx=[&](int i){
		if(i<K)return i;
		else return RND.NextInt(bg[i],ed[i]-1);
	};
	for(auto e:tc.E)
		edge.push_back(P(Idx(e.first),Idx(e.second)));
	Output(N,edge.size(),K,edge);
}
vector <P> E;
int sz;
PP extension_right(int L,int R,int v,double r,int RR)//今見せかけ [L,R] のやつを長さ 1/r 倍にする
{
	int t=(R-L)*(1.0-r)/r-1;
	t=max(t,1);
	t=min(t,RR-R);
	E.push_back(P(L,sz));
	E.push_back(P(sz,R+t));
	E.push_back(P(L,sz+1));
	E.push_back(P(sz+1,R+t));
	E.push_back(P(sz+1,sz));
	E.push_back(P(sz,v));
	sz+=2;
	return PP(P(L,R+t),sz-1);
}
PP extension_left(int L,int R,int v,double r,int LL)//今見せかけ [L,R] のやつを長さ 1/r 倍にする
{
	int t=(R-L)*(1.0-r)/r-1;
	t=max(t,1);
	t=min(t,L-LL);
	E.push_back(P(L-t,sz));
	E.push_back(P(sz,R));
	E.push_back(P(L-t,sz+1));
	E.push_back(P(sz+1,R));
	E.push_back(P(sz,sz+1));
	E.push_back(P(v,sz));
	sz+=2;
	return PP(P(L-t,R),sz-1);
}
TestCase Anti_Rand_Sub(int N,int K,int t,double r)// 4*t < K, 9*t < N 推奨
{
	E.clear();
	sz=K;
	int a=0,b=t;
	int v=sz++;
	E.push_back(P(a,v));
	E.push_back(P(v,b));
	while(b-a<=2*t+2)
	{
		PP p=extension_right(a,b,v,r,K-1);
		a=p.first.first;
		b=p.first.second;
		v=p.second;
	}
	int c=t+1,d=b;
	int u=sz++;
	E.push_back(P(c,u));
	E.push_back(P(u,d));
	while(c>0)
	{
		PP p=extension_left(c,d,u,r,0);
		c=p.first.first;
		d=p.first.second;
		u=p.second;
	}
	E.push_back(P(u,v));
	return TestCase{N,K,E};
}
void Anti_Rand(int N,int K,int t,double r)
{
	OutputTestCase(Anti_Rand_Sub(N,K,t,r));
}
PP extension_right2(int L,int R,int v,double r,int RR)//今見せかけ [L,R] のやつを長さ 1/r 倍にする
{
	int t=(R-L)*(1.0-r)/r-1;
	t=max(t,1);
	t=min(t,RR-R);
	E.push_back(P(L,sz));
	E.push_back(P(sz,R+t));
	E.push_back(P(L,sz+1));
	E.push_back(P(sz+1,R+t));
	E.push_back(P(sz+1,sz));
	E.push_back(P(sz,sz+1));
	E.push_back(P(sz,v));
	E.push_back(P(v,sz));
	sz+=2;
	return PP(P(L,R+t),sz-1);
}
PP extension_left2(int L,int R,int v,double r,int LL)//今見せかけ [L,R] のやつを長さ 1/r 倍にする
{
	int t=(R-L)*(1.0-r)/r-1;
	t=max(t,1);
	t=min(t,L-LL);
	E.push_back(P(L-t,sz));
	E.push_back(P(sz,R));
	E.push_back(P(L-t,sz+1));
	E.push_back(P(sz+1,R));
	E.push_back(P(sz+1,sz));
	E.push_back(P(sz,sz+1));
	E.push_back(P(v,sz));
	E.push_back(P(sz,v));
	sz+=2;
	return PP(P(L-t,R),sz-1);
}
TestCase Anti_Rand_DAG_Sub(int N,int K,int t,double r,int M)// 4*t < K, 9*t < N 推奨
{
	E.clear();
	sz=K;
	int v,c,d;
	{
		int a=0,b=t;
		v=sz++;
		E.push_back(P(a,v));
		E.push_back(P(v,b));
		while(b-a<=2*t+2)
		{
			PP p=extension_right(a,b,v,r,K-1);
			a=p.first.first;
			b=p.first.second;
			v=p.second;
		}
		c=t+1,d=b;
	}
	int u=sz++;
	E.push_back(P(c,u));
	E.push_back(P(u,d));
	while(c>0)
	{
		PP p=extension_left(c,d,u,r,0);
		c=p.first.first;
		d=p.first.second;
		u=p.second;
	}
	E.push_back(P(u,v));
	M-=E.size();
	vector <int> ALL;
	for(int i=0;i<K;i++) ALL.push_back(i);
	for(int i=sz;i<N;i++) ALL.push_back(i);
	vector <P> F;
	set <P> es;
	for(int i=0;i+1<K;i++) es.insert(P(i,i+1));
	for(int i=0;i<K;i++) es.insert(P(i,i));
	while(M--)
	{
		int a,b;
		do
		{
			a=RND.NextInt(0,(int) ALL.size()-1);
			b=RND.NextInt(0,(int) ALL.size()-1);
			if(a>b) swap(a,b);
		}while(es.count(P(a,b)));
		F.push_back(P(ALL[a],ALL[b]));
		es.insert(P(a,b));
	}
	for(int i=0;i<int(E.size());i++) F.push_back(E[i]);
	return TestCase{N,K,F};
}
void Anti_Rand_DAG(int N,int K,int t,double r,int M)
{
	OutputTestCase(Anti_Rand_DAG_Sub(N,K,t,r,M));
}
void WorstCase(int N,int K,int M){
	vector<P> edge;
	for(int i=K;i+1<N;i++){
		edge.emplace_back(i,i+1);
		M--;
	}
	KnuthShuffle(edge);
	ensure(0<M);
	queue<vector<int>> p,q;
	{
		vector<int> waf(K),relka(N-K);
		for(int i=0;i<K;i++)
			waf[i]=i;
		for(int i=0;i<N-K;i++)
			relka[i]=i+K;
		p.push(waf);
		q.push(relka);
	}
	vector<pair<vector<int>,vector<int>>> canEdge;
	while(!p.empty()){
		vector<int> waf=p.front(),relka=q.front();
		p.pop();q.pop();
		int a=waf.size(),b=relka.size();
		if(a==0)continue;
		if(b==0)continue;
		if(a==1||b==1||M<3){
			if(RND.NextUInt(2))
				canEdge.emplace_back(waf,relka);
			else
				canEdge.emplace_back(relka,waf);
			continue;
		}
		int x=(a-RND.NextUInt(2))/2,y=(b-1)/2;
		edge.emplace_back(relka[y],waf[x]);
		edge.emplace_back(waf[x],relka[y+1]);
		M-=2;
		vector<int> w1,w2,r1,r2;
		for(int j=0;j<x;j++)
			w1.push_back(waf[j]);
		for(int j=x+1;j<a;j++)
			w2.push_back(waf[j]);
		for(int j=0;j<y;j++)
			r1.push_back(relka[j]);
		for(int j=y+2;j<b;j++)
			r2.push_back(relka[j]);
		p.push(w1);p.push(w2);
		q.push(r1);q.push(r2);
	}
	set<P> es;
	for(auto e:edge)
		es.insert(e);
	if(!canEdge.empty())
		for(int i=0;i<2000000&&1<M;i++){
			int a=RND.NextUInt(canEdge.size());
			const vector<int>&waf=canEdge[a].first,&relka=canEdge[a].second;
			int u=waf[RND.NextUInt(waf.size())],v=relka[RND.NextUInt(relka.size())];
			if(es.count(P(u,v))==0){
				M--;
				es.insert(P(u,v));
				edge.emplace_back(u,v);
			}
		}
	edge.emplace_back(N-1,K);
	Output(N,edge.size(),K,edge);
}
struct BIT{
	vector<int> buf;
	int s;
	void Init(int n){
		buf.resize(s=n,-1);
	}
	void Update(int i,int v){
		for(;i<s;i+=((i+1)&(-i-1)))
			buf[i]=max(buf[i],v);
	}
	int GetMax(int i){
		int ret=-1;
		for(;i>=0;i-=((i+1)&(-i-1)))
			ret=max(ret,buf[i]);
		return ret;
	}
};
void WorstCase_B(int N,int K,int M){
	ensure(K>=2);
	ensure(M>=4);
	//int a=(M-1)/3,b=a,c=M-1-a-b;
	const double waf=2;
	ensure(M>=1+(N-K)-1+K*waf+K*waf);
	int a=M-1-K*waf-K*waf,b=K*waf,c=K*waf;
	vector<P> edge;
	set<P> es;
	vector<vector<int>> g(N);
	const auto AddEdge=[&](int u,int v){
		edge.push_back(P(u,v));
		es.insert(P(u,v));
		g[u].push_back(v);
	};
	for(int i=K;i<N-1;i++)
		AddEdge(i,i+1);
	int relka=edge.size();
	ensure(int(es.size())<=a);
	while(int(es.size())<a){
		int u,v;
		do{
			u=RND.NextInt(K,N-1);
			v=RND.NextInt(K,N-1);
			if(u>v)swap(u,v);
		}while(u==v||es.count(P(u,v)));
		AddEdge(u,v);
	}
	/*
	while(int(es.size())<a+b){
		int u,v;
		do{
			u=RND.NextInt(0,K-2);
			v=RND.NextInt(K,N-1);
		}while(u==v||es.count(P(u,v)));
		AddEdge(u,v);
	}
	vector<int> reach(N,-1);
	for(int i=0;i<N;i++){
		if(i<K)reach[i]=i;
		for(auto to:g[i])
			reach[to]=max(reach[to],reach[i]);
	}
	while(int(es.size())<a+b+c){
		int u,v;
		do{
			u=RND.NextInt(K,N-1);
			v=RND.NextInt(reach[u]+1,K-1);
		}while(u==v||es.count(P(u,v)));
		AddEdge(u,v);
	}*/
	{
		BIT b1,b2;
		b1.Init(N-K);
		b2.Init(N-K);
		while(int(es.size())<a+b+c){
			if(RND.NextUInt(2)){
				int u=RND.NextUInt(N-K);
				int v=RND.NextInt(max(1,b1.GetMax(u)+1),K-1);
				if(es.count(P(u+K,v)))continue;
				AddEdge(u+K,v);
				b2.Update(N-K-1-u,K-1-v);
			}else{
				int u=RND.NextUInt(N-K);
				int v=K-1-RND.NextInt(max(1,b2.GetMax(N-K-1-u)+1),K-1);
				if(es.count(P(v,u+K)))continue;
				AddEdge(v,u+K);
				b1.Update(u,v);
			}
		}
	}
	KnuthShuffle(edge,0,relka);
	KnuthShuffle(edge,relka,edge.size());
	const auto Weight=[&](P p){
		if(K<=min(p.first,p.second))
			return -1;
		else if(p.first<K){
			return p.first;
		}else if(p.second<K){
			return K-1-p.second;
		}else
			assert(false);
	};
	sort(edge.begin()+relka,edge.end(),[&](P p,P q){
		return Weight(p)<Weight(q);
	});
	vector<int> reach(N,-1);
	for(int i=0;i<N;i++){
		if(i<K)reach[i]=i;
		for(auto to:g[i])
			reach[to]=max(reach[to],reach[i]);
	}
	while(int(es.size())<M){
		int u,v;
		do{
			do{
				u=RND.NextInt(K,N-1);
			}while(reach[u]==-1);
			v=RND.NextInt(0,reach[u]);
		}while(u==v||es.count(P(u,v)));
		AddEdge(u,v);
	}
	//return TestCase{N,K,edge};
	Output(N,edge.size(),K,edge);
}
/*
void WorstCase_C(int N,int M){
	ensure(N%2==0);
	int K=N/2;
	vector<P> edge;
	for(int i=K;i<N;i++){
		if(i+1<N)
			edge.emplace_back(i,i+1);
		edge.emplace_back(i,i-K);
		if(i+1<N)
			edge.emplace_back(i-K,i+1);
	}
	set<P> es;
	for(auto e:edge)
		es.insert(e);
	while(int(es.size())<M-1){
		int u=RND.NextInt(K,N-1),v=RND.NextInt(K,N-1);
		if(u>v)swap(u,v);
		if(u==v||es.count(P(u,v)))continue;
		es.insert(P(u,v));
		edge.emplace_back(u,v);
	}
	KnuthShuffle(edge);
	const auto Weight=[&](P p){
		if(K<=min(p.first,p.second))
			return -1;
		else if(p.first<K){
			return p.first;
		}else if(p.second<K){
			return K-1-p.second;
		}else
			assert(false);
	};
	sort(edge.begin(),edge.end(),[&](P p,P q){
		return Weight(p)<Weight(q);
	});
	edge.emplace_back(N-1,K);
	Output(N,edge.size(),K,edge);
}
*/
/*
void WorstCase_B(int N,int K,int M){//not worst
	vector<P> edge;
	for(int i=K;i+1<N;i++){
		edge.emplace_back(i,i+1);
		M--;
	}
	KnuthShuffle(edge);
	ensure(0<M);
	int a=0,b=K,c=K,d=N;
	while(1<=b-a&&2<=d-c&&3<=M){
		int mid=(a+b)/2;
		edge.emplace_back(c,mid);
		edge.emplace_back(mid,c+1);
		M-=2;
		a=mid+1;
		c+=2;
	}
	set<P> es;
	for(auto e:edge)
		es.insert(e);
	while(2<=M){
		int u=RND.NextInt(0,K-1);
		int v=RND.NextInt(c,d-1);
		if(es.count(P(u,v))==0){
			es.insert(P(u,v));
			edge.emplace_back(u,v);
			M--;
		}
	}
	edge.emplace_back(N-1,K);
	Output(N,edge.size(),K,edge);
}*/
void Sample1()
{
	int n=6,k=3;
	vector <P> es;
	es.push_back(P(3,4));
	es.push_back(P(5,0));
	es.push_back(P(4,5));
	es.push_back(P(5,3));
	es.push_back(P(1,4));
	Output(n,es.size(),k,es,false);
}
void Sample2()
{
	int n=4,k=2;
	vector <P> es;
	es.push_back(P(1,1));
	Output(n,es.size(),k,es,false);
}
void Sample3()
{
	int n=4,k=2;
	vector <P> es;
	es.push_back(P(1,3));
	es.push_back(P(2,0));
	es.push_back(P(3,2));
	Output(n,es.size(),k,es,false);
}
signed main(signed argc, char* argv[])
{
	registerGen(argc,argv,1);
	RND.Reset(2018);
	
	taskName="sample";
	TaskNumber=-1;
	Sample1();
	Sample2();
	Sample3();
	
	bool debug=false;
	//debug=true;
	
	if(!debug){
	
	taskName="01";
	{
		TaskNumber=1;
		All_Path(1,1,true);
		All_Path(9,123,true);
		All_Path(100,300,true);
		All_Path(45,130,false);
		All_Path(100,283,false);
		All_Path(100,278,true);
		All_Path(100,300,false);
	}
	taskName="02";
	{
		TaskNumber=2;
		
		const int Nmax=100,Mmax=300;
		
		//Rand(Nmax,1,Mmax);
		Rand(Nmax,10,Mmax);
		//Rand(Nmax,1000,Mmax);
		
		Many_Path(Nmax,10);
		//Many_Path(2000,1000);
		
		//Cycle(2000,1,20,Mmax);
		Cycle(Nmax,10,20,Mmax);
		
		//for(int i=0;i<2;i++) Two_Path_A(Nmax,Nmax/2);
		Two_Path_A(Nmax,Nmax/2);
		//for(int i=0;i<2;i++) Two_Path_B(Nmax,Nmax/4,RND.NextInt(Mmax*0.8,Mmax));
		Two_Path_B(Nmax,Nmax/4,RND.NextInt(Mmax*0.8,Mmax));
		
		//Rand_DAG_A(Nmax,100,RND.NextInt(Mmax*0.8,Mmax),1,false);
		Rand_DAG_A(Nmax,10,RND.NextInt(Mmax*0.8,Mmax),10,false);
		
		//Rand_DAG_B(Nmax,Nmax/2,RND.NextInt(Mmax*0.8,Mmax),1,false);
		Rand_DAG_B(Nmax,Nmax/2,RND.NextInt(Mmax*0.8,Mmax),5,false);
		
		{
			TestCase tc=Anti_Rand_Sub(Nmax*0.9,45,8,0.98);
			OutputTestCase(CombineTwoTest(Two_Path_B_Sub(Nmax*0.1,3,RND.NextInt(Mmax*0.8,Mmax)-tc.E.size()),tc));
		}
		
		OutputTestCase(CombineTwoTest(Two_Path_B_Sub(Nmax*0.1,3,RND.NextInt(Mmax*0.8,Mmax)/2),Anti_Rand_DAG_Sub(Nmax*0.9,45,8,0.98,Mmax/2)));
	}

	taskName="03";
	{
		TaskNumber=3;
		
		const int Nmax=1000,Mmax=5000;
		
		//Rand(Nmax,1,Mmax);
		Rand(Nmax,10,Mmax);
		Rand(Nmax,100,Mmax);
		//Rand(Nmax,1000,Mmax);
		
		Many_Path(Nmax,100);
		//Many_Path(2000,1000);
		
		//Cycle(2000,1,20,Mmax);
		Cycle(Nmax,10,20,Mmax);
		
		//for(int i=0;i<2;i++) Two_Path_A(Nmax,Nmax/2);
		Two_Path_A(Nmax,Nmax/2);
		//for(int i=0;i<2;i++) Two_Path_B(Nmax,Nmax/4,RND.NextInt(Mmax*0.8,Mmax));
		Two_Path_B(Nmax,Nmax/4,RND.NextInt(Mmax*0.8,Mmax));
		
		//Rand_DAG_A(Nmax,100,RND.NextInt(Mmax*0.8,Mmax),1,false);
		Rand_DAG_A(Nmax,100,RND.NextInt(Mmax*0.8,Mmax),100,false);
		
		//Rand_DAG_B(Nmax,Nmax/2,RND.NextInt(Mmax*0.8,Mmax),1,false);
		Rand_DAG_B(Nmax,Nmax/2,RND.NextInt(Mmax*0.8,Mmax),5,false);
		
		{
			TestCase tc=Anti_Rand_Sub(Nmax*0.9,450,100,0.98);
			OutputTestCase(CombineTwoTest(Two_Path_B_Sub(Nmax*0.1,30,RND.NextInt(Mmax*0.8,Mmax)-tc.E.size()),tc));
		}
		
		OutputTestCase(CombineTwoTest(Two_Path_B_Sub(Nmax*0.1,30,RND.NextInt(Mmax*0.8,Mmax)/2),Anti_Rand_DAG_Sub(Nmax*0.9,450,100,0.98,Mmax/2)));
	}
	
	taskName="04";
	{
		TaskNumber=4;
		
		const int Nmax=30000,Kmax=1000,Mmax=50000;
		
		//Rand(Nmax,1,Mmax);
		//Rand(Nmax,10,Mmax);
		Rand(Nmax,100,Mmax);
		Rand(Nmax,1000,Mmax);
		
		Many_Path(Nmax,100);
		//Many_Path(Nmax,1000);
		
		//Cycle(Nmax,1,100,Mmax);
		Cycle(Nmax,10,100,Mmax);
		
		//for(int i=0;i<2;i++) Two_Path_A(Nmax,Kmax);
		Two_Path_A(Nmax,Kmax);
		//for(int i=0;i<2;i++) Two_Path_B(Nmax,Kmax,RND.NextInt(Mmax*0.8,Mmax));
		Two_Path_B(Nmax,Kmax,RND.NextInt(Mmax*0.8,Mmax));
		
		//Rand_DAG_A(Nmax,100,RND.NextInt(Mmax*0.8,Mmax),1,false);
		Rand_DAG_A(Nmax,100,RND.NextInt(Mmax*0.8,Mmax),100,false);
		Rand_DAG_A(Nmax,1000,RND.NextInt(Mmax*0.8,Mmax),1000,false);
		
		//Rand_DAG_B(Nmax,Kmax,RND.NextInt(Mmax*0.8,Mmax),1,false);
		Rand_DAG_B(Nmax,Kmax,RND.NextInt(Mmax*0.8,Mmax),5,false);
		Rand_DAG_B(Nmax,Kmax,RND.NextInt(Mmax*0.8,Mmax),10,false);
		
		{
			TestCase tc=Two_Path_A_Sub(Nmax*0.9,Kmax-100);
			OutputTestCase(CombineTwoTest(Two_Path_B_Sub(Nmax*0.1,100,RND.NextInt(Mmax*0.8,Mmax)-tc.E.size()),tc));
		}
		
		{
			TestCase tc=Anti_Rand_Sub(Nmax*0.2,500,100,0.98);
			OutputTestCase(CombineTwoTest(Two_Path_B_Sub(Nmax*0.8,500,RND.NextInt(Mmax*0.8,Mmax)-tc.E.size()),tc));
		}
		
		OutputTestCase(CombineTwoTest(Two_Path_B_Sub(Nmax*0.5,Kmax*0.5,RND.NextInt(Mmax*0.8,Mmax)/2),Anti_Rand_DAG_Sub(Nmax*0.5,Kmax*0.5,100,0.98,Mmax/2)));
		
		WorstCase(Nmax,Kmax,Mmax);
		WorstCase_B(Nmax,Kmax,Mmax);
	}
	
	taskName="05";
	{
		TaskNumber=5;
		
		const int Nmax=300000,Mmax=500000;
		
		//Rand(Nmax,1,Mmax);
		//Rand(Nmax,10,Mmax);
		Rand(Nmax,100,Mmax);
		Rand(Nmax,1000,Mmax);
		Rand(Nmax,2000,Mmax);
		//Rand(Nmax,10000,Mmax);
		//Rand(Nmax,100000,Mmax);
		
		Many_Path(Nmax,100);
		//Many_Path(Nmax,1000);
		//Many_Path(Nmax,10000);
		
		//Cycle(Nmax,1,100,Mmax);
		//Cycle(Nmax,10,100,Mmax);
		Cycle(Nmax,100,100,Mmax);
		
		//for(int i=0;i<2;i++) Two_Path_A(Nmax,Nmax/2);
		Two_Path_A(Nmax,Nmax/2);
		//for(int i=0;i<2;i++) Two_Path_B(Nmax,Nmax/4,RND.NextInt(Mmax*0.8,Mmax));
		Two_Path_B(Nmax,Nmax/4,RND.NextInt(Mmax*0.8,Mmax));
		
		//Rand_DAG_A(Nmax,100,RND.NextInt(Mmax*0.8,Mmax),1,false);
		Rand_DAG_A(Nmax,100,RND.NextInt(Mmax*0.8,Mmax),100,false);
		Rand_DAG_A(Nmax,1000,RND.NextInt(Mmax*0.8,Mmax),1000,false);
		Rand_DAG_A(Nmax,10000,RND.NextInt(Mmax*0.8,Mmax),10000,false);
		
		//Rand_DAG_A(Nmax,100,RND.NextInt(Mmax*0.8,Mmax),100,true);
		Rand_DAG_A(Nmax,1000,RND.NextInt(Mmax*0.8,Mmax),1000,true);
		Rand_DAG_A(Nmax,10000,RND.NextInt(Mmax*0.8,Mmax),10000,true);
		
		//Rand_DAG_B(Nmax,Nmax/2,RND.NextInt(Mmax*0.8,Mmax),1,false);
		Rand_DAG_B(Nmax,Nmax/2,RND.NextInt(Mmax*0.8,Mmax),5,false);
		Rand_DAG_B(Nmax,Nmax/2,RND.NextInt(Mmax*0.8,Mmax),10,false);
		Rand_DAG_B(Nmax,Nmax/2,RND.NextInt(Mmax*0.8,Mmax),20,false);
		
		//Rand_DAG_B(Nmax,Nmax/2,RND.NextInt(Mmax*0.8,Mmax),50,true);
		Rand_DAG_B(Nmax,Nmax/2,RND.NextInt(Mmax*0.8,Mmax),100,true);
		Rand_DAG_B(Nmax,Nmax/2,RND.NextInt(Mmax*0.8,Mmax),200,true);
		
		OutputTestCase(CombineTwoTest(Rand_Sub(Nmax*0.5,100,RND.NextInt(Mmax*0.8,Mmax)*0.5),Two_Path_A_Sub(Nmax*0.5,Nmax*0.5/2)));
		
		OutputTestCase(CombineTwoTest(Rand_Sub(Nmax*0.5,2000,RND.NextInt(Mmax*0.8,Mmax)*0.5),Two_Path_A_Sub(Nmax*0.5,Nmax*0.5/2)));
		
		{
			TestCase tc=Two_Path_A_Sub(Nmax*0.9,50000);
			OutputTestCase(CombineTwoTest(Two_Path_B_Sub(Nmax*0.1,10000,RND.NextInt(Mmax*0.8,Mmax)-tc.E.size()),tc));
		}
		
		OutputTestCase(CombineTwoTest(Rand_Sub(Nmax*0.5,100,RND.NextInt(Mmax*0.8,Mmax)*0.5),Two_Path_B_Sub(Nmax*0.5,Nmax*0.5/4,RND.NextInt(Mmax*0.8,Mmax)*0.5)));
		
		OutputTestCase(CombineTwoTest(Rand_Sub(Nmax*0.5,2000,RND.NextInt(Mmax*0.8,Mmax)*0.5),Two_Path_B_Sub(Nmax*0.5,Nmax*0.5/4,RND.NextInt(Mmax*0.8,Mmax)*0.5)));
		
		{
			TestCase tc=Two_Path_B_Sub(Nmax*0.5,Nmax*0.5/4,RND.NextInt(Mmax*0.8,Mmax)*0.5);
			OutputTestCase(CombineTwoTest(Two_Path_B_Sub(Nmax*0.5,10000,RND.NextInt(Mmax*0.8,Mmax)-tc.E.size()),tc));
		}
		
		//for(int i=0;i<2;i++)
		//	Anti_Rand(Nmax,Nmax/3,100,0.98);
		
		{
			TestCase tc=Anti_Rand_Sub(Nmax*0.2,50000,100,0.98);
			OutputTestCase(CombineTwoTest(Rand_Sub(Nmax*0.8,100,RND.NextInt(Mmax*0.8,Mmax)-tc.E.size()),tc));
		}
		
		{
			TestCase tc=Anti_Rand_Sub(Nmax*0.2,50000,100,0.98);
			OutputTestCase(CombineTwoTest(Rand_Sub(Nmax*0.8,2000,RND.NextInt(Mmax*0.8,Mmax)-tc.E.size()),tc));
		}
		
		{
			TestCase tc=Anti_Rand_Sub(Nmax*0.2,50000,100,0.98);
			OutputTestCase(CombineTwoTest(Two_Path_B_Sub(Nmax*0.8,50000,RND.NextInt(Mmax*0.8,Mmax)-tc.E.size()),tc));
		}
		
		//for(int i=0;i<2;i++)
		//	Anti_Rand_DAG(Nmax,Nmax/3,100,0.98,Mmax);
		
		OutputTestCase(CombineTwoTest(Rand_Sub(Nmax*0.5,100,RND.NextInt(Mmax*0.8,Mmax)/2),Anti_Rand_DAG_Sub(Nmax/2,50000,100,0.98,Mmax/2)));

		OutputTestCase(CombineTwoTest(Rand_Sub(Nmax*0.5,2000,RND.NextInt(Mmax*0.8,Mmax)/2),Anti_Rand_DAG_Sub(Nmax/2,50000,100,0.98,Mmax/2)));
		
		OutputTestCase(CombineTwoTest(Two_Path_B_Sub(Nmax*0.5,30000,RND.NextInt(Mmax*0.8,Mmax)/2),Anti_Rand_DAG_Sub(Nmax/2,50000,100,0.98,Mmax/2)));
		
		Rand_DAG_A(Nmax,20000,Mmax,10000,true);
		WorstCase(Nmax,20000,Mmax);
		WorstCase_B(Nmax,20000,Mmax);
	}
	
	}else{
	}
}
