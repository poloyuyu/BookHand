template<ll K>
vector<pair<ll, ll>> misra_merge(vector<pair<ll, ll>> a, vector<pair<ll, ll>> b) {
  for (auto [x, c]: b) {
    auto it = find_if(a.begin(), a.end(), [&](auto i){return i.first == x;});
    if (it != a.end()) it->second += c;
    else a.emplace_back(x, c);
  }
  while (a.size() >= K) {
    ll mini = LLONG_MAX;
    for (auto [x, c]: a) mini = min(mini, c);
    vector<pair<ll, ll>> tmp;
    for (auto [x, c]: a) {
      c -= mini;
      if (c > 0) tmp.emplace_back(x, c);
    }
    a = tmp;
  }
  return a;
}
template<ll K>
struct segment_tree_misra_gries : segment_tree<vector<pair<ll, ll>>, misra_merge<K>> {
  using base = segment_tree<vector<pair<ll, ll>>, misra_merge<K>>;
  using base::base;
};
