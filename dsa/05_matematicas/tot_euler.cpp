//Tested
//sieve
vector<ll> tot_euler(ll n){
    vector<ll> tot(n+1, 0);
    for_(i, n+1) tot[i]=i;
    for(ll i = 2; i <= n; ++i){
        if(tot[i] == i){
            for(ll j = i; j <= n; j+=i) tot[j] -= tot[j]/i;
        }
    }
    return tot;
}
//for a simply n
ll tot_euler(ll n){
    ll n_ = n, tot = n;
    for(ll i = 2; i*i <= n; i++){
        if(n_%i==0){
            tot-=tot/i;
            while(n_%i==0) n_/=i;
        }
    }
    if(n_>1) tot-=tot/n_;
    return tot;
}
