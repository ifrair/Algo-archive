// Knut l-r dp optimization method
// The problem is how to cut logs at the cutting points
// if you have to pay the length of the rib for each cut

#pragma once

#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n, l;
    cin >> n >> l;
    n += 2;
    vector<int> x(n);
    x[0] = 0;
    x[n - 1] = l;
    for (int i = 1; i < n - 1; ++i) {
        cin >> x[i];
    }
    vector<vector<int>> dp(n, vector<int>(n, 1e9));
    vector<vector<int>> opt(n, vector<int>(n, -1));
    for (int r = 0; r < n; ++r) {
        dp[r][r] = 0;
        opt[r][r] = r;
        if (r > 0) {
            dp[r - 1][r] = 0;
            opt[r - 1][r] = r;
        }
        for (int l = r - 2; l >= 0; --l) {
            for (int t = opt[l][r - 1]; t <= opt[l + 1][r]; ++t) {
                if (dp[l][r] > dp[l][t] + dp[t][r]) {
                    dp[l][r] = dp[l][t] + dp[t][r];
                    opt[l][r] = t;
                }
            }
            dp[l][r] += x[r] - x[l];
        }
    }
    cout << dp[0][n - 1];
}

/*
 6 25
 1 3 7 8 11 15
 
 25
 0 1 3 7 8 11 15    15 25
 
 40
 0 1 3 7   7 8 11 15  15 25
 
 55
 0 1 3  3 7  7 8 11  11 15  15 25
 
 62
 */
