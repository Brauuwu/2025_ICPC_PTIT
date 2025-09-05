/*
 * CP Graph & Tree Helper (C++14, dễ đọc)
 * Biên dịch (khuyên dùng):
 *   g++ -std=gnu++14 -O2 -pipe -static -s main.cpp -o main
 */

#include <bits/stdc++.h>
using namespace std;

using ll  = long long;
using pii = pair<int,int>;
using pll = pair<long long,long long>;

/* ===================================================================
    FAST I/O (tùy chọn)
    ===================================================================*/
static inline void fast_io() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
}

/* ===================================================================
    DSU / UNION-FIND: dùng cho nối thành phần, Kruskal,...
    ===================================================================*/
struct DSU {
    vector<int> parent, rnk, sz;

    DSU(int n=0) { init(n); }

    void init(int n) {
        parent.resize(n+1);
        rnk.assign(n+1, 0);
        sz.assign(n+1, 1);
        for (int i=0;i<=n;i++) parent[i]=i;
    }

    int find(int x) {
        if (parent[x]==x) return x;
        return parent[x]=find(parent[x]);
    }

    bool unite(int a, int b) {
        a=find(a); b=find(b);
        if (a==b) return false;
        if (rnk[a]<rnk[b]) swap(a,b);
        parent[b]=a;
        sz[a]+=sz[b];
        if (rnk[a]==rnk[b]) rnk[a]++;
        return true;
    }

    int size(int x) { return sz[find(x)]; }
};

/* ===================================================================
    ĐỒ THỊ VÔ HƯỚNG (cơ bản): BFS, DFS, đếm thành phần liên thông
    ===================================================================*/
struct Graph {
    int n;                            // số đỉnh (1..n)
    vector<vector<int>> adj;          // danh sách kề

    Graph(int n=0): n(n), adj(n+1) {}

    void addEdge(int u, int v, bool undirected=true) {
        adj[u].push_back(v);
        if (undirected) adj[v].push_back(u);
    }

    // BFS: khoảng cách cạnh 1 (unweighted)
    vector<int> bfs(int s) const {
        vector<int> dist(n+1, -1);
        queue<int> q;
        dist[s]=0; q.push(s);
        while(!q.empty()) {
            int u=q.front(); q.pop();
            for (int v: adj[u]) if (dist[v]==-1) {
                dist[v]=dist[u]+1;
                q.push(v);
            }
        }
        return dist;
    }

    // DFS đệ quy
    void dfs_util(int u, vector<int> &vis) const {
        vis[u]=1;
        for (int v: adj[u]) if (!vis[v]) dfs_util(v, vis);
    }

    int connected_components() const {
        vector<int> vis(n+1,0);
        int comp=0;
        for (int i=1;i<=n;i++) if (!vis[i]) {
            comp++;
            dfs_util(i, vis);
        }
        return comp;
    }
};

/* ===================================================================
    TOPOLOGICAL SORT cho đồ thị có hướng (DAG)
    ===================================================================*/
// Kahn (BFS theo bậc vào)
vector<int> topo_kahn(int n, const vector<vector<int>> &g) {
    vector<int> indeg(n+1,0);
    for (int u=1;u<=n;u++)
        for (int v: g[u]) indeg[v]++;

    queue<int> q;
    for (int i=1;i<=n;i++) if (indeg[i]==0) q.push(i);

    vector<int> order;
    while(!q.empty()) {
        int u=q.front(); q.pop();
        order.push_back(u);
        for (int v: g[u]) {
            if (--indeg[v]==0) q.push(v);
        }
    }
    // nếu order.size()<n => có chu trình
    return order;
}

// DFS-based topo
void topo_dfs_util(int u, const vector<vector<int>> &g, vector<int> &vis, vector<int> &stk) {
    vis[u]=1;
    for (int v: g[u]) if (!vis[v]) topo_dfs_util(v,g,vis,stk);
    stk.push_back(u);
}
vector<int> topo_dfs(int n, const vector<vector<int>> &g) {
    vector<int> vis(n+1,0), stk;
    for (int i=1;i<=n;i++) if (!vis[i]) topo_dfs_util(i,g,vis,stk);
    reverse(stk.begin(), stk.end());
    return stk;
}

/* ===================================================================
    ĐƯỜNG ĐI NGẮN NHẤT
    ===================================================================*/
// 1) 0-1 BFS: trọng số chỉ 0 hoặc 1
vector<int> zero_one_bfs(int n, const vector<vector<pair<int,int>>> &g, int s) {
    deque<int> dq;
    const int INF = 1e9;
    vector<int> dist(n+1, INF);
    dist[s]=0; dq.push_front(s);

    while(!dq.empty()) {
        int u = dq.front(); dq.pop_front();
        for (auto e: g[u]) {
            int v=e.first, w=e.second; // w∈{0,1}
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                if (w==0) dq.push_front(v);
                else dq.push_back(v);
            }
        }
    }
    return dist;
}

// 2) Dijkstra: trọng số không âm
vector<ll> dijkstra(int n, const vector<vector<pair<int,int>>> &g, int s) {
    const ll INF = (1LL<<62);
    vector<ll> dist(n+1, INF);
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;
    dist[s]=0; pq.push({0, s});

    while(!pq.empty()) {
        auto top = pq.top(); pq.pop();
        ll du = top.first; int u = top.second;
        if (du != dist[u]) continue;
        for (auto e: g[u]) {
            int v=e.first, w=e.second;
            if (dist[v] > du + w) {
                dist[v] = du + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

/* ===================================================================
    MST (Minimum Spanning Tree)
    ===================================================================*/
struct Edge { int u, v, w; };

// 1) Kruskal: Mặc định đồ thị vô hướng, dùng DSU
ll kruskal(int n, vector<Edge> edges) {
    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b){
        return a.w < b.w;
    });
    DSU dsu(n);
    ll cost=0; int used=0;
    for (const auto &e: edges) {
        if (dsu.unite(e.u, e.v)) {
            cost += e.w;
            used++;
            if (used==n-1) break;
        }
    }
    if (used!=n-1) return -1; // không liên thông
    return cost;
}

// 2) Prim (adjacency list): g[u] = (v, w)
ll prim(int n, const vector<vector<pair<int,int>>> &g, int start=1) {
    const ll INF = (1LL<<62);
    vector<ll> minE(n+1, INF);
    vector<int> used(n+1, 0);
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;
    minE[start]=0; pq.push({0, start});
    ll total = 0; int cnt=0;

    while(!pq.empty()) {
        auto top = pq.top(); pq.pop();
        ll w = top.first; int u = top.second;
        if (used[u]) continue;
        used[u]=1; total += w; cnt++;
        for (auto e: g[u]) {
            int v=e.first, c=e.second;
            if (!used[v] && minE[v] > c) {
                minE[v]=c;
                pq.push({minE[v], v});
            }
        }
    }
    if (cnt!=n) return -1;
    return total;
}

/* ===================================================================
    LOANG (Flood Fill) trên lưới (grid)
    ===================================================================*/
int nRow, nCol;
static int dx4[4]={1,-1,0,0};
static int dy4[4]={0,0,1,-1};
static int dx8[8]={1,1,1,0,0,-1,-1,-1};
static int dy8[8]={1,0,-1,1,-1,1,0,-1};

bool inb(int r, int c) { return (r>=0 && r<nRow && c>=0 && c<nCol); }

// Loang 4 hướng, đếm kích thước vùng có ký tự target
int flood_fill4(vector<string> &a, int sr, int sc, char target, char visited_mark='#') {
    if (!inb(sr,sc) || a[sr][sc]!=target) return 0;
    int cnt=0;
    queue<pii> q; q.push({sr,sc}); a[sr][sc]=visited_mark;
    while(!q.empty()) {
        pii cur=q.front(); q.pop();
        int r=cur.first, c=cur.second; cnt++;
        for (int k=0;k<4;k++) {
            int nr=r+dx4[k], nc=c+dy4[k];
            if (inb(nr,nc) && a[nr][nc]==target) {
                a[nr][nc]=visited_mark;
                q.push({nr,nc});
            }
        }
    }
    return cnt;
}

/* ===================================================================
    LCA (Lowest Common Ancestor) bằng Binary Lifting
    ===================================================================*/
struct LCA {
    int n, LOG;
    vector<vector<int>> up; // up[k][v] = tổ tiên 2^k của v
    vector<int> depth;
    vector<vector<int>> g;

    LCA(int n=0) { if (n) init(n); }

    void init(int n_) {
        n=n_;
        g.assign(n+1, {});
        depth.assign(n+1, 0);
        LOG=1; while((1<<LOG)<=n) LOG++;
        up.assign(LOG, vector<int>(n+1, 0));
    }

    void addEdge(int u, int v) { g[u].push_back(v); g[v].push_back(u); }

    void dfs(int u, int p) {
        up[0][u]=p;
        for (int k=1;k<LOG;k++) {
            int mid = up[k-1][u];
            up[k][u] = mid ? up[k-1][mid] : 0;
        }
        for (int v: g[u]) if (v!=p) {
            depth[v]=depth[u]+1;
            dfs(v, u);
        }
    }

    void build(int root=1) {
        depth[root]=0;
        dfs(root, 0);
    }

    int lift(int u, int k) const {
        for (int i=0;i<LOG;i++) if (k&(1<<i)) u = up[i][u];
        return u;
    }

    int lca(int a, int b) const {
        if (depth[a]<depth[b]) swap(a,b);
        a = lift(a, depth[a]-depth[b]);
        if (a==b) return a;
        for (int k=LOG-1;k>=0;k--) {
            if (up[k][a]!=up[k][b]) {
                a=up[k][a]; b=up[k][b];
            }
        }
        return up[0][a];
    }

    int dist(int a, int b) const {
        int c = lca(a,b);
        return depth[a]+depth[b]-2*depth[c];
    }
};

/* ===================================================================
    CÂY NHỊ PHÂN: build & duyệt, BST cơ bản
    ===================================================================*/
struct Node {
    int val;
    Node *left, *right;
    Node(int v): val(v), left(nullptr), right(nullptr) {}
};

// Build từ (preorder + inorder)
Node* build_pre_in(vector<int> &pre, int pl, int pr,
                    vector<int> &in , int il, int ir,
                    unordered_map<int,int> &pos) {
    if (pl>pr || il>ir) return nullptr;
    int rootVal = pre[pl];
    int k = pos[rootVal];
    int leftLen = k - il;
    Node *root = new Node(rootVal);
    root->left  = build_pre_in(pre, pl+1, pl+leftLen, in, il, k-1, pos);
    root->right = build_pre_in(pre, pl+leftLen+1, pr, in, k+1, ir, pos);
    return root;
}
Node* buildFromPreIn(vector<int> pre, vector<int> in) {
    unordered_map<int,int> pos;
    for (int i=0;i<(int)in.size();i++) pos[in[i]]=i;
    return build_pre_in(pre, 0, (int)pre.size()-1, in, 0, (int)in.size()-1, pos);
}

// Build từ (postorder + inorder)
Node* build_post_in(vector<int> &post, int pl, int pr,
                    vector<int> &in  , int il, int ir,
                    unordered_map<int,int> &pos) {
    if (pl>pr || il>ir) return nullptr;
    int rootVal = post[pr];
    int k = pos[rootVal];
    int leftLen = k - il;
    Node *root = new Node(rootVal);
    root->left  = build_post_in(post, pl, pl+leftLen-1, in, il, k-1, pos);
    root->right = build_post_in(post, pl+leftLen, pr-1, in, k+1, ir, pos);
    return root;
}
Node* buildFromPostIn(vector<int> post, vector<int> in) {
    unordered_map<int,int> pos;
    for (int i=0;i<(int)in.size();i++) pos[in[i]]=i;
    return build_post_in(post, 0, (int)post.size()-1, in, 0, (int)in.size()-1, pos);
}

// Duyệt cây
void preorder(Node* r) { if(!r) return; cout<<r->val<<' '; preorder(r->left); preorder(r->right); }
void inorder (Node* r) { if(!r) return; inorder(r->left); cout<<r->val<<' '; inorder(r->right); }
void postorder(Node* r){ if(!r) return; postorder(r->left); postorder(r->right); cout<<r->val<<' '; }
void levelorder(Node* r){
    if (!r) return;
    queue<Node*> q; q.push(r);
    while(!q.empty()) {
        Node* u=q.front(); q.pop();
        cout<<u->val<<' ';
        if (u->left) q.push(u->left);
        if (u->right) q.push(u->right);
    }
}

// BST cơ bản
Node* bst_insert(Node* r, int x) {
    if (!r) return new Node(x);
    if (x < r->val) r->left  = bst_insert(r->left, x);
    else            r->right = bst_insert(r->right, x);
    return r;
}
bool bst_find(Node* r, int x) {
    if (!r) return false;
    if (r->val==x) return true;
    if (x < r->val) return bst_find(r->left, x);
    return bst_find(r->right, x);
}

/* ===================================================================
    SEGMENT TREE (2 biến thể): 
    1) Range Sum + Point Update
    2) Range Add + Range Sum (Lazy)
    ===================================================================*/
// 1) Segment Tree: range sum + point update
struct SegTree {
    int n;                    // kích thước cây n là power-of-two
    vector<long long> st;     // giá trị nút

    SegTree(int n_=0){ if(n_) init(n_); }

    void init(int n_) {
        n=1; while(n<n_) n<<=1;
        st.assign(2*n, 0);
    }

    void build(const vector<long long> &a) {
        init((int)a.size());
        for (int i=0;i<(int)a.size();i++) st[n+i]=a[i];
        for (int i=n-1;i>=1;i--) st[i]=st[i<<1]+st[i<<1|1];
    }

    // cập nhật điểm: a[idx] = val
    void point_update(int idx, long long val) {
        int p=n+idx; st[p]=val;
        for (p>>=1; p>=1; p>>=1) st[p]=st[p<<1]+st[p<<1|1];
    }

    // tính tổng đoạn [l..r]
    long long range_sum(int l, int r) {
        long long L=0, R=0;
        for (l+=n, r+=n; l<=r; l>>=1, r>>=1) {
            if (l&1) L += st[l++];
            if (!(r&1)) R += st[r--];
        }
        return L+R;
    }
};

// 2) Segment Tree Lazy: range add + range sum
struct SegLazy {
    int n;
    vector<long long> st, lz;

    SegLazy(int n_=0){ if(n_) init(n_); }

    void init(int n_) {
        n=1; while(n<n_) n<<=1;
        st.assign(2*n, 0);
        lz.assign(2*n, 0);
    }

    void build(const vector<long long> &a) {
        init((int)a.size());
        for (int i=0;i<(int)a.size();i++) st[n+i]=a[i];
        for (int i=n-1;i>=1;i--) st[i]=st[i<<1]+st[i<<1|1];
    }

    void apply(int p, long long add, int len) {
        st[p] += add * len;
        lz[p] += add;
    }

    void push(int p, int len) {
        if (lz[p]!=0) {
            int half = len>>1;
            apply(p<<1,   lz[p], half);
            apply(p<<1|1, lz[p], half);
            lz[p]=0;
        }
    }

    void range_add(int l, int r, long long val) { range_add(l,r,val,1,0,n-1); }
    void range_add(int l, int r, long long val, int p, int L, int R) {
        if (r<L || R<l) return;
        if (l<=L && R<=r) { apply(p,val,R-L+1); return; }
        push(p, R-L+1);
        int M=(L+R)>>1;
        range_add(l,r,val,p<<1,  L,  M);
        range_add(l,r,val,p<<1|1,M+1,R);
        st[p] = st[p<<1] + st[p<<1|1];
    }

    long long range_sum(int l, int r) { return range_sum(l,r,1,0,n-1); }
    long long range_sum(int l, int r, int p, int L, int R) {
        if (r<L || R<l) return 0;
        if (l<=L && R<=r) return st[p];
        push(p, R-L+1);
        int M=(L+R)>>1;
        return range_sum(l,r,p<<1,L,M) + range_sum(l,r,p<<1|1,M+1,R);
    }
};

/* ===================================================================
    FENWICK TREE (BIT)
    ===================================================================*/
struct Fenwick {
    int n;
    vector<long long> bit;

    Fenwick(int n_=0){ if(n_) init(n_); }

    void init(int n_) { n=n_; bit.assign(n+1, 0); }

    // cộng v vào a[idx]
    void add(int idx, long long v) {
        for (; idx<=n; idx+=idx&-idx) bit[idx]+=v;
    }

    // tổng prefix [1..idx]
    long long sumPrefix(int idx) {
        long long s=0;
        for (; idx>0; idx-=idx&-idx) s+=bit[idx];
        return s;
    }

    // tổng đoạn [l..r]
    long long rangeSum(int l, int r) {
        if (l>r) return 0;
        return sumPrefix(r) - sumPrefix(l-1);
    }
};

/* ===================================================================
    MAIN DEMO (tùy bài, bật các ví dụ dưới để test)
    ===================================================================*/
int main() {
    fast_io();

    // ===== Ví dụ nhỏ: BFS khoảng cách trên đồ thị vô hướng =====
    /*
    {
        int n=6; Graph G(n);
        G.addEdge(1,2); G.addEdge(1,3); G.addEdge(2,4); G.addEdge(3,5); G.addEdge(5,6);
        vector<int> d = G.bfs(1);
        cout << "dist(1->6) = " << d[6] << "\n"; // 3
    }
    */

    // ===== Ví dụ: Dijkstra =====
    /*
    {
        int n=5;
        vector<vector<pair<int,int>>> g(n+1);
        auto add=[&](int u,int v,int w){ g[u].push_back({v,w}); g[v].push_back({u,w}); };
        add(1,2,4); add(1,3,2); add(3,2,1); add(2,4,5); add(3,5,10);
        vector<ll> d = dijkstra(n,g,1);
        cout << "d[4] = " << d[4] << "\n"; // 8
    }
    */

    // ===== Ví dụ: Kruskal =====
    /*
    {
        int n=4;
        vector<Edge> edges={{1,2,1},{2,3,2},{3,4,3},{1,4,100}};
        cout << "MST = " << kruskal(n, edges) << "\n"; // 6
    }
    */

    // ===== Ví dụ: LCA =====
    /*
    {
        int n=5; LCA T(n);
        T.addEdge(1,2); T.addEdge(1,3); T.addEdge(2,4); T.addEdge(2,5);
        T.build(1);
        cout << "lca(4,5)=" << T.lca(4,5) << "\n"; // 2
        cout << "dist(4,3)=" << T.dist(4,3) << "\n"; // 3
    }
    */

    // ===== Ví dụ: Segment Tree thường =====
    /*
    {
        vector<long long> a={1,2,3,4,5};
        SegTree st; st.build(a);
        cout << st.range_sum(1,3) << "\n"; // 9
        st.point_update(2,10);
        cout << st.range_sum(1,3) << "\n"; // 16
    }
    */

    // ===== Ví dụ: Segment Tree Lazy =====
    /*
    {
        vector<long long> a={1,2,3,4,5};
        SegLazy sl; sl.build(a);
        sl.range_add(1,3,5); // +5 vào [1..3]
        cout << sl.range_sum(0,4) << "\n"; // 30
    }
    */

    // ===== Ví dụ: Fenwick =====
    /*
    {
        Fenwick fw(5);
        for (int i=1;i<=5;i++) fw.add(i, i); // 1..5
        cout << fw.rangeSum(2,4) << "\n"; // 9
    }
    */

    // ===== Ví dụ: Flood Fill 4 hướng =====
    /*
    {
        vector<string> grid = {
            "aab",
            "abb",
            "bbb"
        };
        nRow = (int)grid.size();
        nCol = (int)grid[0].size();
        int sizeComp = flood_fill4(grid, 0, 0, 'a', '#'); // loang từ (0,0) các ô 'a'
        cout << "Size = " << sizeComp << "\n"; // 2
    }
    */

    // ===== Ví dụ: Cây nhị phân từ Pre+In =====
    /*
    {
        vector<int> pre={3,9,20,15,7}, in={9,3,15,20,7};
        Node* root = buildFromPreIn(pre,in);
        inorder(root); cout << "\n";    // 9 3 15 20 7
        preorder(root); cout << "\n";   // 3 9 20 15 7
        postorder(root); cout << "\n";  // 9 15 7 20 3
    }
    */

    // Khi làm bài thật, thay phần trên bằng input/output của đề bài.
    return 0;
}
