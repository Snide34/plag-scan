#include "pattern_matcher.h"
#include <gtest/gtest.h>
#include <algorithm>

// ============================================================================
// KMP Algorithm Tests
// ============================================================================

class KMPTest : public ::testing::Test {
protected:
    KMP kmp;
};

TEST_F(KMPTest, EmptyString) {
    // Edge case: empty pattern
    auto result1 = kmp.search("hello world", "");
    EXPECT_TRUE(result1.empty());
    
    // Edge case: empty text
    auto result2 = kmp.search("", "pattern");
    EXPECT_TRUE(result2.empty());
    
    // Edge case: both empty
    auto result3 = kmp.search("", "");
    EXPECT_TRUE(result3.empty());
}

TEST_F(KMPTest, FullMatch) {
    // Pattern matches entire text
    auto result = kmp.search("hello", "hello");
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 0);
}

TEST_F(KMPTest, NoMatch) {
    // Pattern not found in text
    auto result = kmp.search("hello world", "xyz");
    EXPECT_TRUE(result.empty());
    
    // Pattern longer than text
    auto result2 = kmp.search("hi", "hello");
    EXPECT_TRUE(result2.empty());
}

TEST_F(KMPTest, OverlappingMatches) {
    // Pattern appears multiple times with overlaps
    auto result = kmp.search("aaaa", "aa");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], 0);
    EXPECT_EQ(result[1], 1);
    EXPECT_EQ(result[2], 2);
    
    // Another overlapping case
    auto result2 = kmp.search("abababa", "aba");
    ASSERT_EQ(result2.size(), 3);
    EXPECT_EQ(result2[0], 0);
    EXPECT_EQ(result2[1], 2);
    EXPECT_EQ(result2[2], 4);
}

TEST_F(KMPTest, MultipleNonOverlappingMatches) {
    // Pattern appears multiple times without overlap
    auto result = kmp.search("hello world hello universe", "hello");
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], 0);
    EXPECT_EQ(result[1], 12);
}

TEST_F(KMPTest, BuildFailureFunction) {
    // Test failure function construction
    auto lps = kmp.buildFailureFunction("ABABCABAB");
    std::vector<int> expected = {0, 0, 1, 2, 0, 1, 2, 3, 4};
    EXPECT_EQ(lps, expected);
    
    // Test with repeating pattern
    auto lps2 = kmp.buildFailureFunction("AAAA");
    std::vector<int> expected2 = {0, 1, 2, 3};
    EXPECT_EQ(lps2, expected2);
}

TEST_F(KMPTest, SingleCharacter) {
    auto result = kmp.search("aaa", "a");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], 0);
    EXPECT_EQ(result[1], 1);
    EXPECT_EQ(result[2], 2);
}

// ============================================================================
// Rabin-Karp Algorithm Tests
// ============================================================================

class RabinKarpTest : public ::testing::Test {
protected:
    RabinKarp rk;
};

TEST_F(RabinKarpTest, EmptyString) {
    // Edge case: empty pattern
    auto result1 = rk.search("hello world", "");
    EXPECT_TRUE(result1.empty());
    
    // Edge case: empty text
    auto result2 = rk.search("", "pattern");
    EXPECT_TRUE(result2.empty());
    
    // Edge case: both empty
    auto result3 = rk.search("", "");
    EXPECT_TRUE(result3.empty());
}

TEST_F(RabinKarpTest, FullMatch) {
    // Pattern matches entire text
    auto result = rk.search("hello", "hello");
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 0);
}

TEST_F(RabinKarpTest, NoMatch) {
    // Pattern not found in text
    auto result = rk.search("hello world", "xyz");
    EXPECT_TRUE(result.empty());
    
    // Pattern longer than text
    auto result2 = rk.search("hi", "hello");
    EXPECT_TRUE(result2.empty());
}

TEST_F(RabinKarpTest, OverlappingMatches) {
    // Pattern appears multiple times with overlaps
    auto result = rk.search("aaaa", "aa");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], 0);
    EXPECT_EQ(result[1], 1);
    EXPECT_EQ(result[2], 2);
    
    // Another overlapping case
    auto result2 = rk.search("abababa", "aba");
    ASSERT_EQ(result2.size(), 3);
    EXPECT_EQ(result2[0], 0);
    EXPECT_EQ(result2[1], 2);
    EXPECT_EQ(result2[2], 4);
}

TEST_F(RabinKarpTest, MultipleNonOverlappingMatches) {
    // Pattern appears multiple times without overlap
    auto result = rk.search("hello world hello universe", "hello");
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], 0);
    EXPECT_EQ(result[1], 12);
}

TEST_F(RabinKarpTest, SearchMultiple) {
    std::string text = "the quick brown fox jumps over the lazy dog";
    std::vector<std::string> patterns = {"the", "fox", "dog", "cat"};
    
    auto results = rk.searchMultiple(text, patterns);
    
    // "the" appears twice
    ASSERT_EQ(results["the"].size(), 2);
    EXPECT_EQ(results["the"][0], 0);
    EXPECT_EQ(results["the"][1], 31);
    
    // "fox" appears once
    ASSERT_EQ(results["fox"].size(), 1);
    EXPECT_EQ(results["fox"][0], 16);
    
    // "dog" appears once
    ASSERT_EQ(results["dog"].size(), 1);
    EXPECT_EQ(results["dog"][0], 40);
    
    // "cat" doesn't appear
    EXPECT_TRUE(results["cat"].empty());
}

TEST_F(RabinKarpTest, HashCollisionHandling) {
    // Test with strings that might cause hash collisions
    // The verify step should catch false positives
    auto result = rk.search("abcdefghijklmnop", "xyz");
    EXPECT_TRUE(result.empty());
}

// ============================================================================
// PatternMatcher Interface Tests
// ============================================================================

TEST(PatternMatcherTest, MatchDensityKMP) {
    KMP kmp;
    
    // Pattern appears twice in text of length 10
    // Density = (2 * 2) / 10 = 0.4
    double density = kmp.matchDensity("aabaabaaab", "aa");
    EXPECT_NEAR(density, 0.4, 0.001);
    
    // No matches
    double density2 = kmp.matchDensity("hello", "xyz");
    EXPECT_DOUBLE_EQ(density2, 0.0);
    
    // Full match
    double density3 = kmp.matchDensity("test", "test");
    EXPECT_DOUBLE_EQ(density3, 1.0);
}

TEST(PatternMatcherTest, MatchDensityRabinKarp) {
    RabinKarp rk;
    
    // Pattern appears twice in text of length 10
    double density = rk.matchDensity("aabaabaaab", "aa");
    EXPECT_NEAR(density, 0.4, 0.001);
    
    // Empty pattern
    double density2 = rk.matchDensity("hello", "");
    EXPECT_DOUBLE_EQ(density2, 0.0);
}

TEST(PatternMatcherTest, KMPvsRabinKarpConsistency) {
    KMP kmp;
    RabinKarp rk;
    
    std::string text = "abracadabra";
    std::string pattern = "abra";
    
    auto kmpResult = kmp.search(text, pattern);
    auto rkResult = rk.search(text, pattern);
    
    EXPECT_EQ(kmpResult, rkResult);
}

// ============================================================================
// Edge Cases and Stress Tests
// ============================================================================

TEST(EdgeCaseTest, LongPattern) {
    KMP kmp;
    std::string text = "short";
    std::string pattern = "this is a very long pattern";
    
    auto result = kmp.search(text, pattern);
    EXPECT_TRUE(result.empty());
}

TEST(EdgeCaseTest, RepeatingCharacters) {
    RabinKarp rk;
    std::string text(1000, 'a');
    std::string pattern(10, 'a');
    
    auto result = rk.search(text, pattern);
    EXPECT_EQ(result.size(), 991); // 1000 - 10 + 1
}

TEST(EdgeCaseTest, SpecialCharacters) {
    KMP kmp;
    std::string text = "Hello, World! @#$%";
    std::string pattern = "@#$";
    
    auto result = kmp.search(text, pattern);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 14);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
