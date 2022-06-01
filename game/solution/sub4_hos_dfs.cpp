/*
  Task: game (ioi-2018-game)
  Author: hos

  O((N + M) K), DFS
*/

#include "game.h"
#include <bitset>
#include <vector>

using std::bitset;
using std::vector;

constexpr int MAX_N = 30000;

int N, K;

int k;
vector<vector<int>> graph;
vector<bitset<MAX_N>> visited;

bool dfs(int x) {
  if (x <= k) {
    return true;
  }
  if (!visited[k][x]) {
    visited[k].set(x);
    for (const int y : graph[x]) {
      if (dfs(y)) {
        return true;
      }
    }
  }
  return false;
}

void init(int N, int K) {
  ::N = N;
  ::K = K;
  graph.assign(N, vector<int>());
  visited.assign(K, bitset<MAX_N>());
  for (int x = 0; x < K; ++x) {
    visited[x].set(x);
  }
}

int add_teleporter(int u, int v) {
  graph[u].push_back(v);
  for (k = 0; k < K; ++k) {
    if (visited[k][u]) {
      if (dfs(v)) {
        return 1;
      }
    }
  }
  return 0;
}
