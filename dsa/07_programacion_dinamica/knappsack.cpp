void solve() {
  ll n, w;
  cin >> n >> w;
  vector<pair<ll, ll>> v(n);
  for (auto& [a, b] : v) cin >> a >> b;
  vector<ll> ans(w + 2, 0);
  ll ans_ = 0;
  for (auto [vi, wi] : v) {
    for (ll j = w; j >= wi; j--) {
      ans[j] = max(ans[j], ans[j - wi] + vi);
      ans_ = max(ans_, ans[j]);
    }
  }
  cout << ans_ << endl;
}