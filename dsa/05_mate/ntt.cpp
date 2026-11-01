#include<bits/stdc++.h>

using namespace std;

#define ll long long
#define for_(i, n) for(ll i = 0; i < n; ++i)

//TESTED

const ll mod = 998244353, generator = 3, rootn = 3; // for N max = 2^23
/*more modelus primes of the form p = c*2^m + 1 (generators calculated with algorithm in math section): 
const ll mod = 7340033, generator = 3, rootn = 3; // for N max = 2^20
const ll mod = 167772161, generator = 3, rootn = 3; // for N max = 2^25
const ll mod = 469762049, generator = 3, rootn = 3; // for N max = 2^26
*/

ll binpow(ll a, ll e){
    ll ans = 1;
    while(e > 0){
        if(e&1) ans = (ans * a)%mod;
        a = (a*a)%mod;
        e>>=1;
    }
    return ans;
}

ll inv(ll a){return binpow(a, mod-2);}


void ntt(vector<ll> &c, bool invert){
    ll N = 1;
    while(N < c.size())N<<=1;
    c.resize(N);
    ll cnt = 0;
    for(ll j = 1; j < N; ++j){
        ll bit = N>>1;
        while(bit&cnt){
            cnt^=bit;
            bit>>=1;
        }
        cnt^=bit;
        if(cnt < j) swap(c[cnt], c[j]);
    }

    for(ll n = 2; n <= N; n<<=1){
        ll wn = binpow(rootn, (mod-1)/n);
        if(invert) wn = inv(wn);
        for(ll currblock = 0; currblock < N; currblock+=n){
            ll w = 1;
            for(ll i = 0; i < n/2; ++i){
                ll u = c[currblock + i]%mod, v = (w*c[currblock + i + n/2])%mod;
                c[currblock + i] = (u + v)%mod;
                c[currblock + i + n/2] = ((u - v)%mod + mod)%mod;
                w = (w*wn)%mod;
            }
        }
    }
    if(invert){
        for(ll &x : c) x = (x*inv(N))%mod;
    }
}

vector<ll> conv(vector<ll> c1, vector<ll> c2){
    ll N=1;
    while(N <= (c1.size() + c2.size()-1ll)) N<<=1;
    c1.resize(N); c2.resize(N);
    ntt(c1, false);
    ntt(c2, false);
    vector<ll> ans(N, 0);
    for_(i, N) ans[i] = (c1[i] * c2[i])%mod;
    ntt(ans, true);
    return ans;
}