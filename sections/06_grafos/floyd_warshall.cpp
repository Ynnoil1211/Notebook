// 3. Floyd-Warshall (All-Pairs Shortest Path en O(V^3))
void floyd_warshall(int n, vector<vll>& d) {
  // Precondicion: d[i][i] = 0, d[u][v] = min_peso (o INF si
  // no hay arista)
  for (int k = 1; k <= n; ++k)
    for (int i = 1; i <= n; ++i)
      if (d[i][k] < INF)
        for (int j = 1; j <= n; ++j)
          if (d[k][j] < INF)
            d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
}
