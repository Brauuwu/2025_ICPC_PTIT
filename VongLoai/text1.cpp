/*
 * Data Structures & Algorithms (C++14)
 * Biên dịch:
 *   g++ -std=gnu++14 -O2 -pipe -static -s main.cpp -o main
 */

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

/* ================================================================
    1. PRIORITY QUEUE (Heap)
   ================================================================ */
// Mặc định priority_queue trong C++ là max-heap
// Muốn min-heap thì dùng greater<>
void demo_priority_queue() {
    priority_queue<int> maxpq;                        // max-heap
    priority_queue<int, vector<int>, greater<int>> minpq; // min-heap

    vector<int> nums = {5, 1, 9, 3};
    for (int x: nums) { maxpq.push(x); minpq.push(x); }

    cout << "Max-heap top: " << maxpq.top() << "\n"; // 9
    cout << "Min-heap top: " << minpq.top() << "\n"; // 1
}

/* ================================================================
    2. DEQUE (double-ended queue)
   ================================================================ */
void demo_deque() {
    deque<int> dq;
    dq.push_back(1); // [1]
    dq.push_front(2);// [2,1]
    dq.push_back(3); // [2,1,3]

    cout << "Front=" << dq.front() << ", Back=" << dq.back() << "\n"; // 2,3

    dq.pop_front(); // [1,3]
    dq.pop_back();  // [1]
    cout << "Now front=" << dq.front() << "\n"; // 1
}

/* ================================================================
    3. MONOTONIC QUEUE (Sliding Window Min/Max)
   ================================================================ */
// Tìm min/max trên mỗi cửa sổ dài k
vector<int> sliding_window_max(const vector<int> &a, int k) {
    deque<int> dq; vector<int> res;
    for (int i=0;i<(int)a.size();i++) {
        while(!dq.empty() && a[dq.back()]<=a[i]) dq.pop_back();
        dq.push_back(i);
        if (dq.front()<=i-k) dq.pop_front();
        if (i>=k-1) res.push_back(a[dq.front()]);
    }
    return res;
}

/* ================================================================
    4. DISJOINT SET UNION (Union-Find)
   ================================================================ */
struct DSU {
    vector<int> p, r;
    DSU(int n=0){ init(n); }
    void init(int n){ p.resize(n+1); r.assign(n+1,0); iota(p.begin(),p.end(),0); }
    int find(int x){ return p[x]==x?x:p[x]=find(p[x]); }
    bool unite(int a,int b){
        a=find(a); b=find(b); if(a==b) return false;
        if(r[a]<r[b]) swap(a,b);
        p[b]=a; if(r[a]==r[b]) r[a]++;
        return true;
    }
};

/* ================================================================
    5. SPARSE TABLE (RMQ)
   ================================================================ */
struct SparseTable {
    int n, LOG;
    vector<vector<int>> st;
    vector<int> lg;
    SparseTable(const vector<int>& a) { build(a); }

    void build(const vector<int>& a) {
        n=a.size();
        LOG=32-__builtin_clz(n);
        st.assign(LOG, vector<int>(n));
        lg.assign(n+1,0);
        for(int i=2;i<=n;i++) lg[i]=lg[i/2]+1;

        for(int i=0;i<n;i++) st[0][i]=a[i];
        for(int k=1;k<LOG;k++) {
            for(int i=0;i+(1<<k)<=n;i++) {
                st[k][i]=min(st[k-1][i], st[k-1][i+(1<<(k-1))]);
            }
        }
    }

    int query(int L,int R){ // min trên [L..R]
        int j=lg[R-L+1];
        return min(st[j][L], st[j][R-(1<<j)+1]);
    }
};

/* ================================================================
    6. TRIE (cây tiền tố)
   ================================================================ */
struct Trie {
    struct Node {
        map<char, int> next;
        bool end=false;
    };
    vector<Node> tr;

    Trie(){ tr.push_back(Node()); }

    void insert(const string &s){
        int u=0;
        for(char c:s){
            if(!tr[u].next.count(c)){
                tr[u].next[c]=tr.size();
                tr.push_back(Node());
            }
            u=tr[u].next[c];
        }
        tr[u].end=true;
    }

    bool search(const string &s){
        int u=0;
        for(char c:s){
            if(!tr[u].next.count(c)) return false;
            u=tr[u].next[c];
        }
        return tr[u].end;
    }
};

/* ================================================================
    7. ORDERED SET (thực chất dùng set/multiset trong STL)
   ================================================================ */
void demo_set() {
    set<int> s;
    s.insert(5); s.insert(1); s.insert(3);
    cout << "Min element=" << *s.begin() << "\n"; // 1
    cout << "Has 3? " << (s.count(3)?"YES":"NO") << "\n";
    auto it=s.lower_bound(4); // phần tử >=4
    if(it!=s.end()) cout << "Lower_bound(4)=" << *it << "\n"; // 5
}

/* ================================================================
    MAIN DEMO
   ================================================================ */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Demo priority_queue
    demo_priority_queue();

    // Demo deque
    demo_deque();

    // Demo sliding window max
    vector<int> a={1,3,-1,-3,5,3,6,7};
    auto res=sliding_window_max(a,3);
    cout << "Sliding window max (k=3): ";
    for(int x:res) cout << x << " ";
    cout << "\n";

    // Demo Sparse Table
    vector<int> arr={4,6,1,5,7,3};
    SparseTable st(arr);
    cout << "RMQ(1,4)=" << st.query(1,4) << "\n"; // min từ arr[1..4] = 1

    // Demo Trie
    Trie tr;
    tr.insert("hello");
    tr.insert("world");
    cout << "Search hello=" << tr.search("hello") << "\n"; // 1
    cout << "Search hi=" << tr.search("hi") << "\n";       // 0

    // Demo set
    demo_set();

    return 0;
}

/*
priority_queue: mặc định max-heap, muốn min-heap thì thêm greater<>.

deque: hỗ trợ push_front, push_back, pop_front, pop_back.

sliding_window_max: giải quyết bài toán max/min trong cửa sổ trượt.

DSU: quản lý tập hợp rời nhau, thường dùng trong Kruskal, connected component.

SparseTable: tiền xử lý O(n log n), query min/max O(1).

Trie: lưu nhiều string, hỗ trợ tìm kiếm tiền tố.

set/multiset: cho tập hợp có thứ tự, hỗ trợ lower_bound, upper_bound.
*/