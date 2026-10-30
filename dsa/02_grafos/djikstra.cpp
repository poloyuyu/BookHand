vector<ll> dist;
vector < vector<pair<ll, ll>> g;

void djikstra() {
  priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pair<ll, ll>>> pq;
  pq.push({0, 0});  // nodo_inicio, distancia
  while (q.size()) {
    auto [c, node] = pq.top();
    pq.pop();
    if (dist[node] > c) {
      dist[node] = c;
      for (auto [c_, v_] : g[node]) {
        if (dist[v_] > c + c_) pq.push({c + c_, v_});
      }
    }
  }
}