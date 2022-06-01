/*
  Task: game (ioi-2018-game)
  Author: hos

  O((N + M) K), BFS
*/

#include "game.h"
#include <bitset>
#include <queue>
#include <vector>

using std::bitset;
using std::queue;
using std::vector;

constexpr int MAX_N = 30000;

int N, K;

vector<vector<int>> graph;
vector<bitset<MAX_N>> visited;

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
  for (int k = 0; k < K; ++k) {
    if (visited[k][u]) {
      queue<int> q;
      q.push(v);
      for (; !q.empty(); ) {
        const int x = q.front();
        q.pop();
        if (x <= k) {
          return 1;
        }
        if (!visited[k][x]) {
          visited[k].set(x);
          for (const int y : graph[x]) {
            q.push(y);
          }
        }
      }
    }
  }
  return 0;
}
