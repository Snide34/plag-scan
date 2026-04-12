#include "similarity_engine.h"
#include <algorithm>
#include <vector>

int LCS::lcsLength(const std::string& a, const std::string& b) {
    if (a.empty() || b.empty()) return 0;
    
    size_t n = a.length();
    size_t m = b.length();
    
    // Space optimization: use only two rows
    std::vector<int> prev(m + 1, 0);
    std::vector<int> curr(m + 1, 0);
    
    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) {
                curr[j] = prev[j - 1] + 1;
            } else {
                curr[j] = std::max(prev[j], curr[j - 1]);
            }
        }
        prev = curr;
    }
    
    return curr[m];
}

std::string LCS::lcsSequence(const std::string& a, const std::string& b) {
    if (a.empty() || b.empty()) return "";
    
    size_t n = a.length();
    size_t m = b.length();
    
    // Full DP table for backtracking
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
    
    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    // Backtrack to find sequence
    std::string result;
    size_t i = n, j = m;
    
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            result = a[i - 1] + result;
            --i;
            --j;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }
    
    return result;
}

double LCS::similarityScore(const std::string& a, const std::string& b) {
    if (a.empty() && b.empty()) return 1.0;
    if (a.empty() || b.empty()) return 0.0;
    
    int lcsLen = lcsLength(a, b);
    double denominator = static_cast<double>(a.length() + b.length());
    
    return (2.0 * lcsLen) / denominator;
}
