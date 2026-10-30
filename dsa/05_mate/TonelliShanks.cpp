#include<bits/stdc++.h>

using namespace std;
#define ll long long
#define INF LLONG_MAX

// TESTED


ll binpow(ll a, ll b, ll m){
    ll ans = 1;
    while(b > 0){
        if(b&1) ans = (ans*a)%m;
        a=(a*a)%m;
        b>>=1;
    }
    return ans;
}

ll TonelliShanks(ll n, ll p) {
    n %= p; if (n < 0) n += p;
    if (n == 0) return 0;
    if (binpow(n,(p-1)/2,p) != 1) return -1; // solution does not exist
    if (p % 4 == 3) return binpow(n, (p + 1) / 4, p); // trivial solution, only one factor 2
    ll c = p - 1, z = 2;
    ll s = 0;
    while(c % 2 == 0) ++s, c /= 2;
    // find a non-square mod p
    while(binpow(z,(p-1)/2, p) != p - 1) ++z;
    ll q = binpow(z, c, p); // generator subgroup, |<q>| = 2^s
    ll a = binpow(n, (c+1)/2, p); // candidate root
    ll t = binpow(n, c, p); // error from equation a^2 = n*t, if t=1, a is the root
    ll m = s;
    while(1){
        ll i = 0, temp = t;
        while(i < m && temp != 1){
            temp = (temp*temp)%p;
            i++;
        }
        if(i==0) return a;
        ll exponent = 1ll << (m-i-1);
        ll b = binpow(q, exponent, p);
        a = (a*b)%p;
        t = (((t*b)%p)*b)%p;
        q = (b*b)%p;
        m=i;
    }
}