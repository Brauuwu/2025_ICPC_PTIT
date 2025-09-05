// Build khuyên dùng:
// g++ -std=gnu++14 -O2 -pipe -static -s main.cpp -o main
#include <bits/stdc++.h>
using namespace std;

/* ==========================
    TIỆN ÍCH CƠ BẢN
   ========================== */

using ll = long long;

static inline void fast_io() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
}

/*--- GCD / LCM ---*/
ll gcdll(ll a, ll b) { return b ? gcdll(b, a % b) : a; }
ll lcmll(ll a, ll b) { return a / gcdll(a, b) * b; }

/*--- LŨY THỪA NHANH (không mod) ---*/
ll binpow(ll a, long long e) {
    ll r = 1;
    while (e > 0) {
        if (e & 1) r = r * a;
        a = a * a;
        e >>= 1;
    }
    return r;
}

/*--- LŨY THỪA NHANH MOD ---*/
const int MOD = 1000000007;
long long modpow(long long a, long long e, long long mod = MOD) {
    long long r = 1 % mod;
    a %= mod;
    while (e > 0) {
        if (e & 1) r = (r * a) % mod;
        a = (a * a) % mod;
        e >>= 1;
    }
    return r;
}

/* ==========================
    TÌM KIẾM NHỊ PHÂN (mảng tăng)
   ========================== */

/* Trả về chỉ số của x hoặc -1 nếu không có */
int binary_search_idx(const vector<int>& a, int x) {
    int l = 0, r = (int)a.size() - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (a[m] == x) return m;
        if (a[m] < x) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

/* Wrapper cho lower_bound / upper_bound: trả về vị trí chèn */
int lower_bound_idx(const vector<int>& a, int x) {
    return (int)(lower_bound(a.begin(), a.end(), x) - a.begin());
}
int upper_bound_idx(const vector<int>& a, int x) {
    return (int)(upper_bound(a.begin(), a.end(), x) - a.begin());
}

/* ==========================
    PREFIX SUM (tổng từ 0..i)
   ========================== */
vector<long long> build_prefix_sum(const vector<int>& a) {
    vector<long long> pref(a.size() + 1, 0);
    for (size_t i = 0; i < a.size(); ++i)
        pref[i + 1] = pref[i] + a[i];
    return pref;
}
/* Lấy tổng đoạn [l..r], 0-based */
long long range_sum(const vector<long long>& pref, int l, int r) {
    if (l > r) return 0;
    l = max(l, 0);
    r = min(r, (int)pref.size() - 2);
    return pref[r + 1] - pref[l];
}

/* ==========================
    SÀNG ERATOSTHENES (isPrime)
   ========================== */
struct PrimeSieve {
    int N;
    vector<bool> isPrime;
    vector<int> primes;

    PrimeSieve(int n = 0) { if (n) init(n); }
    void init(int n) {
        N = max(2, n);
        isPrime.assign(N + 1, true);
        isPrime[0] = isPrime[1] = false;
        for (int i = 2; 1LL * i * i <= N; ++i) {
            if (isPrime[i]) {
                for (long long j = 1LL * i * i; j <= N; j += i)
                    isPrime[(int)j] = false;
            }
        }
        primes.clear();
        for (int i = 2; i <= N; ++i)
            if (isPrime[i]) primes.push_back(i);
    }
    bool operator()(int x) const { // isPrime(x)
        if (x < 0 || x > N) return false;
        return isPrime[x];
    }
};

/* ==========================
    SPF (SMALLEST PRIME FACTOR)
    - Phân tích thừa số
    - Đếm ước, tổng ước, phi Euler
   ========================== */
struct SPF {
    int N;
    vector<int> spf;  // spf[x] = ước NT nhỏ nhất của x

    SPF(int n = 0) { if (n) init(n); }
    void init(int n) {
        N = max(2, n);
        spf.assign(N + 1, 0);
        for (int i = 2; i <= N; ++i) {
            if (spf[i] == 0) { // i là prime
                spf[i] = i;
                if (1LL * i * i <= N) {
                    for (long long j = 1LL * i * i; j <= N; j += i) {
                        if (spf[(int)j] == 0) spf[(int)j] = i;
                    }
                }
            }
        }
        spf[1] = 1;
    }

    // Trả về các cặp (p, e) sao cho n = Π p^e
    vector<pair<int,int>> factorize(int n) const {
        vector<pair<int,int>> f;
        if (n <= 1) return f;
        while (n > 1) {
            int p = spf[n], c = 0;
            while (n % p == 0) { n /= p; ++c; }
            f.push_back(make_pair(p, c));
        }
        return f;
    }

    // d(n) = ∏ (e_i + 1)
    long long count_divisors(int n) const {
        if (n == 0) return 0;
        if (n == 1) return 1;
        vector<pair<int,int>> f = factorize(n);
        long long d = 1;
        for (size_t i = 0; i < f.size(); ++i) d *= (f[i].second + 1);
        return d;
    }

    // σ(n) = ∏ (p^(e+1)-1)/(p-1)
    long long sum_divisors(int n) const {
        if (n == 0) return 0;
        if (n == 1) return 1;
        vector<pair<int,int>> f = factorize(n);
        long long s = 1;
        for (size_t i = 0; i < f.size(); ++i) {
            int p = f[i].first, e = f[i].second;
            long long powp = 1;
            for (int j = 0; j < e; ++j) powp *= p; // powp = p^e
            long long term = (powp * p - 1) / (p - 1); // 1 + p + ... + p^e
            s = s * term;
        }
        return s;
    }

    // φ(n) = n ∏ (1 - 1/p)
    long long euler_phi(int n) const {
        if (n == 0) return 0;
        if (n == 1) return 1;
        vector<pair<int,int>> f = factorize(n);
        long long res = n;
        for (size_t i = 0; i < f.size(); ++i) {
            int p = f[i].first;
            res = res / p * (p - 1);
        }
        return res;
    }
};

/* ==========================
    TỔ HỢP MOD (nCk % MOD)
   ========================== */
struct CombMod {
    int N;
    vector<long long> fact, invfact;

    CombMod(int n = 0) { if (n) init(n); }

    void init(int n) {
        N = max(1, n);
        fact.assign(N + 1, 1);
        invfact.assign(N + 1, 1);
        for (int i = 1; i <= N; ++i) fact[i] = (fact[i - 1] * i) % MOD;
        invfact[N] = modpow(fact[N], MOD - 2, MOD); // Fermat: MOD nguyên tố
        for (int i = N; i > 0; --i) invfact[i - 1] = (invfact[i] * i) % MOD;
    }

    long long C(int n, int k) const {
        if (k < 0 || k > n) return 0;
        long long res = fact[n];
        res = (res * invfact[k]) % MOD;
        res = (res * invfact[n - k]) % MOD;
        return res;
    }
};

/* ==========================
    DEMO SỬ DỤNG
   ========================== */

int main() {
    fast_io();

    // Ví dụ input demo:
    // n, m
    // a[0..n-1]
    // q, rồi q dòng mỗi dòng 1 x để tìm kiếm
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    // 1) Prefix sum
    vector<long long> pref = build_prefix_sum(a);
    if (m > 0) {
        cout << "Sum of first " << m << " elements = "
                << range_sum(pref, 0, m - 1) << "\n";
    }

    // 2) Sort + Binary search
    sort(a.begin(), a.end());
    cout << "Sorted a: ";
    for (int i = 0; i < n; ++i) cout << a[i] << ' ';
    cout << "\n";

    int q; cin >> q;
    while (q--) {
        int x; cin >> x;
        int pos = binary_search_idx(a, x);
        cout << "Find " << x << ": ";
        if (pos == -1) cout << "not found, ";
        else cout << "at index " << pos << ", ";
        cout << "lower_bound=" << lower_bound_idx(a, x)
                << ", upper_bound=" << upper_bound_idx(a, x) << "\n";
    }

    // 3) Sàng prime cơ bản (isPrime)
    int limitSieve = 1000000;
    PrimeSieve sieve(limitSieve);
    cout << "Prime check 97: " << (sieve(97) ? "YES" : "NO") << "\n";

    // 4) SPF: phân tích thừa số & các hàm ước
    SPF spf(limitSieve);
    int val = 360; // 2^3 * 3^2 * 5
    vector<pair<int,int>> fac = spf.factorize(val);
    cout << "Factorize " << val << " = ";
    for (size_t i = 0; i < fac.size(); ++i)
        cout << fac[i].first << "^" << fac[i].second << " ";
    cout << "\n";
    cout << "Count divisors d(" << val << ") = " << spf.count_divisors(val) << "\n";
    cout << "Sum divisors sigma(" << val << ") = " << spf.sum_divisors(val) << "\n";
    cout << "Euler phi phi(" << val << ") = " << spf.euler_phi(val) << "\n";

    // 5) GCD/LCM
    ll A = 84, B = 360;
    cout << "gcd(" << A << "," << B << ") = " << gcdll(A, B)
            << ", lcm = " << lcmll(A, B) << "\n";

    // 6) Lũy thừa nhanh
    cout << "2^10 = " << binpow(2, 10) << "\n";
    cout << "2^100 mod 1e9+7 = " << modpow(2, 100) << "\n";

    // 7) nCk mod
    CombMod comb(1000000); // chuẩn bị trước đến 1e6
    cout << "C(10,3) mod = " << comb.C(10, 3) << "\n";

    return 0;
}
