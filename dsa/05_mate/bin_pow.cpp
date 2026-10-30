
#include<bits/stdc++.h>

using namespace std;

#define ll long long
#define ull unsigned long long
#define INF LLONG_MAX
#define NINF LLONG_MIN

const ll mod = 1e9 + 7;

/*
Tested : Tested
*/

ll binpow(ll a, ll b, ll m){
    ll ans = 1;
    while(b > 0){
        if(b&1) ans = (ans*a)%m;
        a=(a*a)%m;
        b>>=1;
    }
    return ans;
}
