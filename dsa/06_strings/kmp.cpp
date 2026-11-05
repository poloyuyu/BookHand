struct KMP {
  vector<int> lps;
  string s;
  KMP(string s) : s(s) {
    int n = s.size();
    lps.assign(n, 0);
    int len = 0, i = 1;
    while (i < n) {
      if (s[i] == s[len]) {
        lps[i++] = ++len;
      } else {
        if (len != 0)
          len = lps[len - 1];
        else
          lps[i++] = 0;
      }
    }
  }
};