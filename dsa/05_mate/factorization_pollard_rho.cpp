#include<bits/stdc++.h>
 
using namespace std;
 
#define ll long long
#define ld long double
#define ull unsigned long long
#define for_(i, n) for(ll i = 0; i < n; ++i)
#define INF LLONG_MAX
#define NINF LLONG_MIN
#define i128 __int128
#define u128 __uint128_t

const ll mod = 1e9 + 7;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

//TESTED up to 1e18, run T(n) < 1e8

ll mult(ll a, ll b, ll m) {
    return (__int128)a * b % m;
}

ll f(ll x, ll c, ll m) {
    return (mult(x, x, m) + c) % m;
}

ll gcd(ll a, ll b){
    while(b) { ll r = a % b; a = b; b = r; }
    return a;
}

ll rho(ll n, ll x0=2, ll c=1) {
    ll x = x0;
    ll y = x0;
    ll g = 1;
    while (g == 1) {
        x = f(x, c, n);
        y = f(y, c, n);
        y = f(y, c, n);
        g = gcd(abs(x - y), n);
    }
    return g;
}

//algorithm for the factorization
ll brent(ll n, ll x0=2, ll c=1) {
    ll x = x0;
    ll g = 1;
    ll xs, y;

    ll m = 128;
    ll l = 1;
    ll iter = 0;
    while (g == 1 && iter<1000) {
        y = x;
        ll q = 1;
        for (ll i = 1; i < l; i++)
            x = f(x, c, n);
        ll k = 0;
        while (k < l && g == 1) {
            xs = x;
            for (ll i = 0; i < m && i < l - k; i++) {
                x = f(x, c, n);
                q = mult(q, abs(y - x), n);
            }
            g = gcd(q, n);
            k += m;
        }
        l *= 2;
        iter++;
    }
    if (g == n) {
        return n;
    }
    return g;
}

ll binpow(ll a, ll b, ll mod){
  ll ans = 1;
  while(b>0){
    if(b&1) ans = (u128)(ans)*a % mod;
    a=(u128)a*a % mod;
    b>>=1;
  }
  return ans;
}

bool checkComposite(ll p, ll a, ll s, ll q){
  ll x = binpow(a, q, p);
  if(x == 1 || x == p-1) return false;
  for(ll i = 1; i < s; ++i){
    x = (u128)x*x % p;
    if(x == p-1) return false;
  }
  return true;
}


bool deterministicMillerRabin(ll p){
  if(p<4) return p==2 || p==3;
  ll s=0,q = p-1;
  while(!(q&1)){
    s++;
    q>>=1;
  }
  for(ll a : {2,3,5,7,11,13,17,19,23,29,31,37}){
    if(p==a) return true;
    if(checkComposite(p, a, s, q)) return false;
  }
  return true;
}


//algorithm to prime factorization
void factorize(ll n, map<ll, ll> &factors){
    if(n==1) return;
    if(deterministicMillerRabin(n)){
        factors[n]++;
        return;
    }
    ll d1, d2, iter=0;
    while(iter<1000){
        uniform_int_distribution<ll> dist(1, n-1);
        ll c = dist(rng);
        ll x0 = dist(rng);
        d1 = brent(n, x0, c), d2 = n/d1;
        if(d1>1 && d1<n) break;
        iter++;
    }
    if(d1==1 || d2==1){
        factors[n]++;
        return;
    }
    factorize(d1, factors);
    factorize(d2, factors);
}