/*
 * TASK: ROS/cdgp001/Increasing subsequences
 * solution 3/2 log n
 * Author: Karol Pokorski
 */

#include <bits/stdc++.h>
#include "perm.h"
using namespace std;

typedef long long int LL;

void apply_gadget(vector<int>& v,
                  const vector<int>& gadget) {
  vector<int> gadget_items;
  for (int x : gadget) if (x > 0) gadget_items.push_back(x);
  sort(gadget_items.begin(), gadget_items.end());

  for (int x : gadget_items)
    for (int& y : v)
      if (y >= x) y++;

  int n_size = v.size() + gadget.size();
  for (int x : gadget) {
    if (x <= 0) v.push_back(n_size + x);
    else v.push_back(x);
  }
}

vector<int> dec2bin(LL x) {
  vector<int> result;
  while (x > 0) { result.push_back(x % 2); x /= 2; }
  reverse(result.begin(), result.end());
  return result;
}

vector<int> permutation(LL k) {
  if (k == 1) return vector<int>();
  if (k == 2) return vector<int>({1});
  if (k == 3) return vector<int>({2, 1});

  vector<int> bin_k = dec2bin(k);
  vector<int> result;
  int p = 1;
  if (bin_k[p] == 0) {
    if (bin_k[p+1] == 0) result = vector<int>({3, 2, 1});
    if (bin_k[p+1] == 1) result = vector<int>({2, 3, 1});
    p += 2;
  }
  else { result = vector<int>({2, 1}); p++; }

  while (p+1 < (int)bin_k.size()) {
    if (bin_k[p] == 0) {
      apply_gadget(result, vector<int>({0}));
      p++;
    }
    else {
      if (bin_k[p+1] == 0)
        apply_gadget(result, vector<int>({-1, 1, 0}));
      else
        apply_gadget(result, vector<int>({-1, 0, 3}));
      p += 2;
    }
  }
  if (p < (int)bin_k.size()) {
    if (bin_k[p] == 0)
      apply_gadget(result, vector<int>({0}));
    if (bin_k[p] == 1)
      apply_gadget(result, vector<int>({0, 1}));
    p++;
  }
  return result;
}

vector<int> construct_permutation(long long k) {
  vector<int> result = permutation(k);
  for(int i=0;i<result.size();i++)
    result[i]--;
  return result;
}

