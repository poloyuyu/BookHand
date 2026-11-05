//Tested

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
