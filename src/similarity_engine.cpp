#include "similarity_engine.h"
#include <algorithm>

SimilarityEngine::SimilarityEngine(double lcsWeight, double saWeight)
    : lcsWeight_(lcsWeight), saWeight_(saWeight) {
}

double SimilarityEngine::combinedScore(const std::string& a, const std::string& b) {
    if (a.empty() && b.empty()) return 1.0;
    if (a.empty() || b.empty()) return 0.0;
    
    // LCS-based score
    double lcsScore = lcs_.similarityScore(a, b);
    
    // Suffix array-based score (longest common substring ratio)
    std::string lcs = sa_.longestCommonSubstring(a, b);
    double saScore = 0.0;
    if (!lcs.empty()) {
        double maxLen = std::max(a.length(), b.length());
        saScore = lcs.length() / maxLen;
    }
    
    // Weighted combination
    double combined = lcsWeight_ * lcsScore + saWeight_ * saScore;
    
    // Normalize to [0, 1]
    return std::min(1.0, std::max(0.0, combined));
}

std::vector<std::pair<int, int>> SimilarityEngine::highlightMatches(const std::string& a, const std::string& b) {
    std::vector<std::pair<int, int>> matches;
    
    if (a.empty() || b.empty()) return matches;
    
    // Get all common substrings
    std::vector<std::string> commons = sa_.allCommonSubstrings(a, b, 3);
    
    // Find positions in string a
    for (const auto& common : commons) {
        size_t pos = 0;
        while ((pos = a.find(common, pos)) != std::string::npos) {
            matches.push_back({static_cast<int>(pos), static_cast<int>(pos + common.length())});
            pos++;
        }
    }
    
    // Sort and merge overlapping spans
    if (!matches.empty()) {
        std::sort(matches.begin(), matches.end());
        
        std::vector<std::pair<int, int>> merged;
        merged.push_back(matches[0]);
        
        for (size_t i = 1; i < matches.size(); ++i) {
            if (matches[i].first <= merged.back().second) {
                merged.back().second = std::max(merged.back().second, matches[i].second);
            } else {
                merged.push_back(matches[i]);
            }
        }
        
        matches = merged;
    }
    
    return matches;
}
