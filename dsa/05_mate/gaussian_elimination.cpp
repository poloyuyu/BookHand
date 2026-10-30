#include<bits/stdc++.h>
#include"bin_pow.cpp"
using namespace std;

#define ll long long
#define ull unsigned long long
#define INF LLONG_MAX
#define NINF LLONG_MIN

const double EPS = 1e-18;
const int MOD = 1e9+7;

//Tested

bool gauss_jordan(vector<vector<ll>> A, vector<ll> &x) {
  ll n = A.size(), m = A[0].size() - 1;
 
  vector<ll> where(m, -1);
  for (ll col = 0, row = 0; col < m && row < n; ++col) {
    ll pivot = row;
    for (ll i = row; i < n; ++i){
      if (abs(A[i][col]) > abs(A[pivot][col])) pivot = i;
    }
    if(!A[pivot][col]) continue;
    for (ll i = col; i <= m; ++i) swap(A[pivot][i], A[row][i]);
    where[col] = row;
 
    for(ll i = 0; i < n; ++i){
      ll invm = binpow(A[row][col], MOD - 2, MOD);
      if(i != row) {
        ll c = (A[i][col] * invm) % MOD;
        for (ll j = col; j <= m; ++j) A[i][j] = ((A[i][j] - A[row][j] * c) % MOD + MOD) % MOD;
      }
    }
    ++row;
  }
 
  x.assign(m, 0);
  for (ll i = 0; i < m; ++i){
    if (where[i] != -1){
      ll invm = binpow(A[where[i]][i], MOD - 2, MOD);
      x[i] = (A[where[i]][m] * invm) % MOD;
    }
  }
  for(ll i=0; i<n; ++i){
    ll sum = 0;
    for(ll j=0; j<m; ++j) sum += (x[j] * A[i][j])%MOD;
    if(abs(sum - A[i][m]) > 0) return false; //No solution
  }
  return true;//Valid solution, trivial or infinity
