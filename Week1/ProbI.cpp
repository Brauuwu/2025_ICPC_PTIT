#include <bits/stdc++.h>
using namespace std;

const int MAX = 500;
bool isPrime[MAX + 1];
vector<int> primes;

#define ll long long

// Sàng Eratosthenes để tìm các số nguyên tố <= MAX
void sieve() {
    fill(isPrime, isPrime + MAX + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i <= MAX; i++) {
        if (isPrime[i]) {
            primes.push_back(i);
            for (int j = i * 2; j <= MAX; j += i) {
                isPrime[j] = false;
            }
        }
    }
}

// Hàm đếm số mũ của p trong n! (dùng chia nhiều lần)
int exponent_in_factorial(int n, int p) {
    int count = 0;
    while (n > 0) {
        count += n / p;
        n /= p;
    }
    return count;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    sieve();  // Khởi tạo danh sách các số nguyên tố

    int n, k;
    while (cin >> n >> k) {
        map<int, int> expo;

        for (int p : primes) {
            int e = exponent_in_factorial(n, p)
                    - exponent_in_factorial(k, p)
                    - exponent_in_factorial(n - k, p);
            if (e > 0)
                expo[p] = e;
        }

        ll result = 1;
        for (const pair<int, int>& p : expo) {
            int prime = p.first;
            int e = p.second;
            result *= (e + 1);
        }

        cout << result << '\n';
    }
}
