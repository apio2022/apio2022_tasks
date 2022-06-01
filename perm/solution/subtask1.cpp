#include <bits/stdc++.h>
#include "perm.h"
using namespace std;

vector<int> construct_permutation(long long k) {
  if(k>90) return {};
  vector<int> perm(k-1);
  iota(perm.begin(),perm.end(),0);
  reverse(perm.begin(),perm.end());
  return perm;
}

