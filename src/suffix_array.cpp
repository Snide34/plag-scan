#include "similarity_engine.h"
#include <algorithm>
#include <vector>
#include <map>

std::vector<int> SuffixArray::build(const std::string& s) {
    int n = s.length();
    if (n == 0) return {};
    
    std::vector<int> sa(n);
    std::vector<int> rank(n);
    std::vector<int> tmp(n);
    
    // Initial ranking by first character
    for (int i = 0; i < n; ++i) {
        sa[i] = i;
        rank[i] = s[i];
    }
    
    // Prefix doubling
    for (int k = 1; k < n; k *= 2) {
        auto cmp = [&](int i, int j) {
            if (rank[i] != rank[j]) return rank[i] < rank[j];
            int ri = (i + k < n) ? rank[i + k] : -1;
            int rj = (j + k < n) ? rank[j + k] : -1;
            return ri < rj;
        };
        
        std::sort(sa.begin(), sa.end(), cmp);
        
        tmp[sa[0]] = 0;
        for (int i = 1; i < n; ++i) {
            tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);
        }
        
        rank = tmp;
    }
    
    return sa;
}

std::vector<int> SuffixArray::buildLCP(const std::string& s, const std::vector<int>& sa) {
    int n = s.length();
    if (n == 0) return {};
    
    std::vector<int> lcp(n, 0);
    std::vector<int> rank(n);
    
    for (int i = 0; i < n; ++i) {
        rank[sa[i]] = i;
    }
    
    int h = 0;
    for (int i = 0; i < n; ++i) {
        if (rank[i] > 0) {
            int j = sa[rank[i] - 1];
            while (i + h < n && j + h < n && s[i + h] == s[j + h]) {
                ++h;
            }
            lcp[rank[i]] = h;
            if (h > 0) --h;
        }
    }
    
    return lcp;
}

std::string SuffixArray::longestCommonSubstring(const std::string& a, const std::string& b) {
    if (a.empty() || b.empty()) return "";
    
    // Concatenate with separator
    std::string combined = a + "#" + b;
    int n = combined.length();
    int splitPos = a.length();
    
    std::vector<int> sa = build(combined);
    std::vector<int> lcp = buildLCP(combined, sa);
    
    int maxLen = 0;
    int maxPos = 0;
    
    for (int i = 1; i < n; ++i) {
        // Check if adjacent suffixes are from different strings
        bool fromA1 = sa[i - 1] < splitPos;
        bool fromA2 = sa[i] < splitPos;
        
        if (fromA1 != fromA2 && lcp[i] > maxLen) {
            maxLen = lcp[i];
            maxPos = sa[i];
        }
    }
    
    return combined.substr(maxPos, maxLen);
}

std::vector<std::string> SuffixArray::allCommonSubstrings(const std::string& a, const std::string& b, int minLen) {
    if (a.empty() || b.empty()) return {};
    
    std::string combined = a + "#" + b;
    int n = combined.length();
    int splitPos = a.length();
    
    std::vector<int> sa = build(combined);
    std::vector<int> lcp = buildLCP(combined, sa);
    
    std::vector<std::string> result;
    std::map<std::string, bool> seen;
    
    for (int i = 1; i < n; ++i) {
        bool fromA1 = sa[i - 1] < splitPos;
        bool fromA2 = sa[i] < splitPos;
        
        if (fromA1 != fromA2 && lcp[i] >= minLen) {
            std::string substr = combined.substr(sa[i], lcp[i]);
            if (seen.find(substr) == seen.end()) {
                seen[substr] = true;
                result.push_back(substr);
            }
        }
    }
    
    return result;
}
