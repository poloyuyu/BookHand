void cdq(ll l, ll r, vector<pt>& v) {
  if (r - l <= 1) return;
  ll m = (l + r) / 2;
  cdq(l, m, v);
  cdq(m, r, v);
  vector<pt> tmp;
  ll a = l, b = m;
  while (a < m && b < r) {
    if (v[a].y < v[b].y) {
      st.add(v[a].z, 1);
      tmp.push_back(v[a++]);
    } else {
      ans[v[b].id] += st.sum(v[b].z - 1);
      tmp.push_back(v[b++]);
    }
  }
  ll record = a;
  while (a < m) tmp.push_back(v[a++]);
  while (b < r) {
    ans[v[b].id] += st.sum(v[b].z - 1);
    tmp.push_back(v[b++]);
  }
  for (ll i = l; i < record; i++) st.add(v[i].z, -1);
  for (ll i = l; i < r; i++) v[i] = tmp[i - l];
}
/*
sort(v, v + n, [](const pt &a, const pt &b) {
    if (a.x != b.x) return a.x < b.x;
    if (a.y != b.y) return a.y > b.y; // Opposite of merge condition!
    return a.z > b.z;
});
*/
