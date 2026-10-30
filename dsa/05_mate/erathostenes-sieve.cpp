#include<bits/stdc++.h>

using namespace std;

#define ll long long

//Tested

vector<bool> erathostenesSieve(ll n){
  vector<bool> isprime(n+1, true); vector<ll> primes;
  isprime[0] = isprime[1] = false;
  for(ll i = 2; i <= n; (i==2 ? i++ : i+=2)){
    if(isprime[i]){
      for(ll j = i*i; j <= n; j+=i) isprime[j] = false;
      primes.push_back(i);
    }
  }
  return isprime;
}