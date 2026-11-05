//TESTED
#include"gcd.cpp"
#include"bin_pow.cpp"
#include"tot_euler.cpp"
#include"primitive_root.cpp"

vector<ll> DiscreteRoot(ll n,ll k,ll a){
    if(a==0) return {0};
    ll g = generator_(n);

    ll sq = (ll)sqrt(n) + 1;
    vector<pair<ll, ll>> dec(sq);
    for(ll i = 1; i <= sq; ++i)
        dec[i-1] = {binpow(g, i * sq * k % (n - 1), n), i};
    sort(dec.begin(), dec.end());
    ll h0 = -1;
    for_(i, sq) {
        ll my = binpow(g, i * k % (n - 1), n) * a % n;
        auto it = lower_bound(dec.begin(), dec.end(), make_pair(my, 0));
        if (it != dec.end() && it->first == my) {
            h0 = it->second * sq - i;
            break;
        }
    }
    if(h0 == -1) return {-1};

    ll delta = (n-1)/gcd(k, n-1);
    vector<ll> x;
    for (ll h = h0 % delta; h < n-1; h += delta)
        x.push_back(binpow(g, h, n));
    sort(x.begin(), x.end());
    return x;
}
