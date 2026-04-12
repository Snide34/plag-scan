#ifndef SIMILARITY_ENGINE_H
#define SIMILARITY_ENGINE_H

#include <string>
#include <vector>
#include <utility>

class LCS {
public:
    int lcsLength(const std::string& a, const std::string& b);
    std::string lcsSequence(const std::string& a, const std::string& b);
    double similarityScore(const std::string& a, const std::string& b);
};

class SuffixArray {
public:
    std::vector<int> build(const std::string& s);
    std::vector<int> buildLCP(const std::string& s, const std::vector<int>& sa);
    std::string longestCommonSubstring(const std::string& a, const std::string& b);
    std::vector<std::string> allCommonSubstrings(const std::string& a, const std::string& b, int minLen = 4);
};

class SimilarityEngine {
public:
    SimilarityEngine(double lcsWeight = 0.5, double saWeight = 0.5);
    double combinedScore(const std::string& a, const std::string& b);
    std::vector<std::pair<int, int>> highlightMatches(const std::string& a, const std::string& b);

private:
    double lcsWeight_;
    double saWeight_;
    LCS lcs_;
    SuffixArray sa_;
};

#endif // SIMILARITY_ENGINE_H
