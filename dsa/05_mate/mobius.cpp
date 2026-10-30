
#include<bits/stdc++.h>

using namespace std;

#define ll long long
#define ull unsigned long long
#define for_(i, n) for(ll i = 0; i < n; ++i)
#define INF LLONG_MAX
#define NINF LLONG_MIN

const ll mod = 1e9 + 7; 

//Tested

vector<ll> mobius(ll n){
    vector<ll> mob(n+1, 1);
    mob[1] = 1;
    vector<bool> isprime(n+1, true);
    isprime[0]=isprime[1] = false;
    for(ll i = 2; i < n+1; i++){
        if(isprime[i]){
            for(ll j = i; j < n+1; j+=i){
                mob[j] *= -1;
                isprime[j] = false;
            }
            for(ll j = i*i; j < n+1; j += i*i) mob[j] = 0;
        }
    }
    return mob;
}