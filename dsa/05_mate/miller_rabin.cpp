#include<bits/stdc++.h>

using namespace std;

#define u128 __uint128_t
#define ll long long

//Tested

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

bool millerRabin(ll p, ll maxIter=5){
  if(p<4) return p==2 || p==3;
  ll s=0,q = p-1;
  while(!(q&1)){
    s++;
    q>>=1;
  }
  for(ll i = 0; i < maxIter; ++i){
    ll a = 2 + rand() % (p-3);
    if(checkComposite(p, a, s, q)) return false;
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
