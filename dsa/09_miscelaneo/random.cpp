mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
mt19937_64 rng64(chrono::steady_clock::now().time_since_epoch().count());

// Número aleatorio entero [a, b]
int random(int a, int b) {
    return uniform_int_distribution<int>(a, b)(rng);
}
// Número aleatorio long long [a, b]
ll random(ll a, ll b) {
    return uniform_int_distribution<ll>(a, b)(rng64);
}
// shuffle a un vector v
shuffle(v.begin(), v.end(), rng);

