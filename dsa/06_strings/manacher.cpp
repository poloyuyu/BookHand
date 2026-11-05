vector<int> manacher_odd(string s) {
  int n = s.size();
  s = "$" + s + "^";
  vector<int> p(n + 2);
  int l = 0, r = 1;
  for (int i = 1; i <= n; i++) {
    if (i <= r) {
      p[i] = min(r - i, p[l + (r - i)]);
    }
    while (s[i - p[i]] == s[i + p[i]]) {
      p[i]++;
    }
    if (i + p[i] > r) {
      l = i - p[i], r = i + p[i];
    }
  }
  return vector<int>(begin(p) + 1, end(p) - 1);
}

vector<int> manacher(string s) {
  string t;
  for (auto c : s) {
    t += string("#") + c;
  }
  auto res = manacher_odd(t + "#");
  /*
  int ans_largo = v[i]-1;
  if(i%2 == 0){
      int indice = i/2;
      int l = indice - (ans_largo/2);
      int r = indice + (ans_largo/2);
  } else {
      int indice = (i+1)/2;
      int l = indice - (ans_largo/2);
      int r = indice + (ans_largo/2)-1;
  }
  palindromo -> s[l , r] <-- r incluido
  */
  return vector<int>(begin(res) + 1, end(res) - 1);
}