#include "similarity_engine.h"
#include <gtest/gtest.h>

// LCS Tests
TEST(LCSTest, IdenticalStrings) {
    LCS lcs;
    std::string a = "hello world";
    std::string b = "hello world";
    
    EXPECT_EQ(lcs.lcsLength(a, b), 11);
    EXPECT_EQ(lcs.lcsSequence(a, b), "hello world");
    EXPECT_DOUBLE_EQ(lcs.similarityScore(a, b), 1.0);
}

TEST(LCSTest, PartialMatch) {
    LCS lcs;
    std::string a = "abcdef";
    std::string b = "acdxef";
    
    int len = lcs.lcsLength(a, b);
    EXPECT_GT(len, 0);
    EXPECT_LT(len, 6);
    
    double score = lcs.similarityScore(a, b);
    EXPECT_GT(score, 0.0);
    EXPECT_LT(score, 1.0);
}

TEST(LCSTest, FullyDifferent) {
    LCS lcs;
    std::string a = "abc";
    std::string b = "xyz";
    
    EXPECT_EQ(lcs.lcsLength(a, b), 0);
    EXPECT_EQ(lcs.lcsSequence(a, b), "");
    EXPECT_DOUBLE_EQ(lcs.similarityScore(a, b), 0.0);
}

TEST(LCSTest, EmptyStrings) {
    LCS lcs;
    
    EXPECT_EQ(lcs.lcsLength("", "test"), 0);
    EXPECT_EQ(lcs.lcsLength("test", ""), 0);
    EXPECT_EQ(lcs.lcsLength("", ""), 0);
    EXPECT_DOUBLE_EQ(lcs.similarityScore("", ""), 1.0);
}

TEST(LCSTest, CommonSubsequence) {
    LCS lcs;
    std::string a = "AGGTAB";
    std::string b = "GXTXAYB";
    
    EXPECT_EQ(lcs.lcsLength(a, b), 4);
    EXPECT_EQ(lcs.lcsSequence(a, b), "GTAB");
}

// SuffixArray Tests
TEST(SuffixArrayTest, BuildSuffixArray) {
    SuffixArray sa;
    std::string s = "banana";
    
    auto result = sa.build(s);
    EXPECT_EQ(result.size(), 6);
}

TEST(SuffixArrayTest, LongestCommonSubstringIdentical) {
    SuffixArray sa;
    std::string a = "testing";
    std::string b = "testing";
    
    std::string lcs = sa.longestCommonSubstring(a, b);
    EXPECT_EQ(lcs, "testing");
}

TEST(SuffixArrayTest, LongestCommonSubstringPartial) {
    SuffixArray sa;
    std::string a = "abcdefgh";
    std::string b = "xyzdefpqr";
    
    std::string lcs = sa.longestCommonSubstring(a, b);
    EXPECT_EQ(lcs, "def");
}

TEST(SuffixArrayTest, LongestCommonSubstringNone) {
    SuffixArray sa;
    std::string a = "abc";
    std::string b = "xyz";
    
    std::string lcs = sa.longestCommonSubstring(a, b);
    EXPECT_TRUE(lcs.empty());
}

TEST(SuffixArrayTest, AllCommonSubstrings) {
    SuffixArray sa;
    std::string a = "abcdefgh";
    std::string b = "xyzabcpqr";
    
    auto result = sa.allCommonSubstrings(a, b, 3);
    EXPECT_GT(result.size(), 0);
    
    bool foundAbc = false;
    for (const auto& s : result) {
        if (s == "abc") foundAbc = true;
    }
    EXPECT_TRUE(foundAbc);
}

// SimilarityEngine Tests
TEST(SimilarityEngineTest, IdenticalStrings) {
    SimilarityEngine engine;
    std::string a = "hello world";
    std::string b = "hello world";
    
    double score = engine.combinedScore(a, b);
    EXPECT_NEAR(score, 1.0, 0.01);
}

TEST(SimilarityEngineTest, PartialSimilarity) {
    SimilarityEngine engine;
    std::string a = "the quick brown fox";
    std::string b = "the slow brown dog";
    
    double score = engine.combinedScore(a, b);
    EXPECT_GT(score, 0.3);
    EXPECT_LT(score, 0.9);
}

TEST(SimilarityEngineTest, FullyDifferent) {
    SimilarityEngine engine;
    std::string a = "abcdef";
    std::string b = "xyz123";
    
    double score = engine.combinedScore(a, b);
    EXPECT_LT(score, 0.2);
}

TEST(SimilarityEngineTest, HighlightMatches) {
    SimilarityEngine engine;
    std::string a = "this is a test string";
    std::string b = "this is another test";
    
    auto matches = engine.highlightMatches(a, b);
    EXPECT_GT(matches.size(), 0);
    
    // Verify spans are valid
    for (const auto& match : matches) {
        EXPECT_GE(match.first, 0);
        EXPECT_LE(match.second, static_cast<int>(a.length()));
        EXPECT_LT(match.first, match.second);
    }
}

TEST(SimilarityEngineTest, EmptyStrings) {
    SimilarityEngine engine;
    
    EXPECT_DOUBLE_EQ(engine.combinedScore("", "test"), 0.0);
    EXPECT_DOUBLE_EQ(engine.combinedScore("test", ""), 0.0);
    EXPECT_DOUBLE_EQ(engine.combinedScore("", ""), 1.0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
