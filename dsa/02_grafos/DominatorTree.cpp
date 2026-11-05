#include<bits/stdc++.h>
using namespace std;
#define ll long long
#define for_(i, n) for(ll i = 0; i < n; i++)
/*
  *Description:* Computes the inmediate dominator of every node in a directed graph in O((|E|+|V|)log(|V|)).
  *Status:* Tested on CSES
*/
struct dominator_tree {
    ll n,l,s;
    vector<ll> tin,par,vis_ord, dsu_mn, dsu_par, sdom, dep,rdom,idom;
    vector<bool> vis;
    vector<vector<ll>> up;
    vector<vector<pair<ll,ll>>> up_mn;
    void dfs(ll u, ll p, vector<vector<ll>> & adj){
        if (vis[u]) return;
        vis[u] = 1;
        dep[u] = dep[p]+1;
        tin[u] = vis_ord.size();
        vis_ord.push_back(u);
        par[u] = p;
        for (int v : adj[u]) dfs(v,u,adj);
    }
    ll find(ll x){
        if (dsu_par[x] == -1) return x;
        if (dsu_par[dsu_par[x]] != -1){
            ll res = find(dsu_par[x]);
            if (tin[res] < tin[dsu_mn[x]]) 
				dsu_mn[x] = res;
			dsu_par[x] = dsu_par[dsu_par[x]];
        }
        assert(dsu_mn[x] != INT32_MAX);
        return dsu_mn[x];
    }
    void get_sdom(vector<vector<ll>> & rev){
        for (ll i = vis_ord.size()-1; i >= 0; i--){
            ll c = vis_ord[i];
            if (c == s) continue;
            for (ll to : rev[c]){
                ll res = find(to);
                if (sdom[c] == -1 || tin[res] < tin[sdom[c]])
                    sdom[c] = res;
            }
            dsu_par[c] = par[c];
            dsu_mn[c] = sdom[c];
            if (sdom[c] == -1) cerr << c << endl;
        }
    }
    void init_up(){
        for_(i, n){
            if (!vis[i]) continue;
            up[i][0] = par[i];
            up_mn[i][0] = {tin[sdom[i]],i};
        }
        for (ll i = 1; i < l; i++) {
			for_(j, n) {
                if (!vis[j]) continue;
				ll h = up[j][i-1];
				if (h == -1) {
					up[j][i] = -1;
					up_mn[j][i] = up_mn[j][i-1];
				} else {
					up[j][i] = up[h][i-1];
					up_mn[j][i] = min(up_mn[j][i-1], up_mn[h][i-1]);
				}
			}
		}
    }
    ll get_min(ll v, ll u){
        ll dist = dep[u]-dep[v];
        pair<ll,ll> ret = up_mn[u][0];
        ll w = u;
        for_(i, l)
            if (dist&(1<<i)){
                ret = min(ret,up_mn[w][i]);
                w = up[w][i];
            }
        assert(w == v);
        return ret.second;
    }
    void get_idom(){
        for (ll c : vis_ord){
            if (c == s) continue;
            rdom[c] = get_min(sdom[c],c);
        }
        for (ll c : vis_ord)
            if (c != s)
                idom[c] = ((rdom[c] == c)?sdom[c]:idom[rdom[c]]);
    }
    dominator_tree(vector<vector<ll>> & adj, vector<vector<ll>> & rev, ll _s = 0, ll logn = 20){
        n = adj.size(); l = logn; s = _s;
        tin.resize(n,INT32_MAX);
        par.resize(n,-1);
        vis.resize(n,0);
        dep.resize(n,0);
        dfs(s,-1,adj);
        dsu_par.resize(n,-1);
        dsu_mn.resize(n,INT32_MAX);
        sdom.resize(n,-1);
        rdom.resize(n,-1);
        idom.resize(n,-1);
        get_sdom(rev);
        up.assign(n,vector<ll>(logn));
        up_mn.assign(n,vector<pair<ll,ll>>(logn));
        init_up();
        get_idom();
    }
};

//aux function, cover dom tree
void dfs(ll &cnt, ll node, vector<vector<ll>> &adj, vector<ll> &tin, vector<ll> &tout){
    tin[node] = cnt++;
    for(ll child : adj[node]) dfs(cnt, child, adj, tin, tout);
    tout[node] = cnt++;
}

//verifiy if u dominates v, for each edge
vector<vector<bool>> Verifiy_u_dom_v(ll n, vector<vector<ll>> &adj, vector<vector<ll>> &rev_adj, vector<pair<ll, ll>> &edges){
    dominator_tree dt(adj, rev_adj);
    vector<vector<ll>> dom_tree(n);
    for_(i, n){
        if(i==0) continue;
        if(dt.vis[i]) dom_tree[dt.idom[i]].push_back(i);
    }
    vector<ll> tin(n, 0), tout(n, 0); ll cnt = 0;
    dfs(cnt, 0, dom_tree, tin, tout);
    vector<vector<bool>> dom_matrix(n, vector<bool>(n, false));
    for_(i, edges.size()){
        auto &[u,v] = edges[i];
        if((tin[u]<=tin[v] && tout[v]<=tout[u])) dom_matrix[u][v] = true;
    }
    return dom_matrix;
}