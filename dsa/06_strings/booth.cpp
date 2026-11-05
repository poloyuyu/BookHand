int Booth(string s) {
  s += s;
  vector<int> t(s.size(), -1);
  int ans = 0;
  int n = s.size();
  for (int i = 1; i < n; i++) {
    char c = s[i];
    int j = t[i - ans - 1];
    while (j != -1 && c != s[ans + j + 1]) {
      if (c < s[ans + j + 1]) {
        ans = i - j - 1;
      }
      j = t[j];
    }
    if (c != s[ans + 1 + j]) {
      if (c < s[ans]) {
        ans = i;
      }
      t[i - ans] = -1;
    } else {
      t[i - ans] = j + 1;
    }
  }
  return ans;
}