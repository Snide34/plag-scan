#include "pattern_matcher.h"

// ============================================================================
// KMP Algorithm Implementation
// ============================================================================

std::vector<int> KMP::buildFailureFunction(const std::string& pattern) {
    size_t m = pattern.length();
    std::vector<int> lps(m, 0); // Longest Proper Prefix which is also Suffix
    
    if (m == 0) {
        return lps;
    }
    
    int len = 0; // Length of previous longest prefix suffix
    size_t i = 1;
    
    // lps[0] is always 0
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                // Try the previous longest prefix suffix
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    
    return lps;
}

std::vector<size_t> KMP::search(const std::string& text, const std::string& pattern) {
    std::vector<size_t> positions;
    
    if (pattern.empty() || text.empty() || pattern.length() > text.length()) {
        return positions;
    }
    
    size_t n = text.length();
    size_t m = pattern.length();
    
    // Build failure function
    std::vector<int> lps = buildFailureFunction(pattern);
    
    size_t i = 0; // Index for text
    size_t j = 0; // Index for pattern
    
    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }
        
        if (j == m) {
            // Pattern found at index i - j
            positions.push_back(i - j);
            j = lps[j - 1];
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    
    return positions;
}
