int bs(vector<ll>& v, int b) {
  int l = 0, r = v.size();
  while (l < r) {
    int mid = (l + r) / 2;
    if (v[mid] >= b)
      r = mid;
    else
      l = mid + 1;
  }
  return l;
}