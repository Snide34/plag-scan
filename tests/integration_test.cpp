#include "preprocessor.h"
#include "pattern_matcher.h"
#include "similarity_engine.h"
#include "scorer.h"
#include "file_io.h"
#include <gtest/gtest.h>
#include <iostream>

class IntegrationTest : public ::testing::Test {
protected:
    FileIO fileIO;
    
    ScorerResult runFullPipeline(const std::string& file1, const std::string& file2, bool isCode) {
        // Read files
        std::string content1 = fileIO.readFile(file1);
        std::string content2 = fileIO.readFile(file2);
        
        // Preprocess
        std::string processed1, processed2;
        if (isCode) {
            CodePreprocessor proc;
            processed1 = proc.preprocess(content1, true);
            processed2 = proc.preprocess(content2, true);
        } else {
            TextPreprocessor proc;
            processed1 = proc.preprocess(content1, false);
            processed2 = proc.preprocess(content2, false);
        }
        
        // Pattern matching
        KMP kmp;
        RabinKarp rk;
        double kmpDensity = kmp.matchDensity(processed1, processed2);
        double rkDensity = rk.matchDensity(processed1, processed2);
        
        // Similarity algorithms
        LCS lcs;
        SuffixArray sa;
        double lcsScore = lcs.similarityScore(processed1, processed2);
        
        std::string longestCommon = sa.longestCommonSubstring(processed1, processed2);
        double saScore = 0.0;
        if (!longestCommon.empty() && !processed1.empty()) {
            saScore = static_cast<double>(longestCommon.length()) / 
                     static_cast<double>(std::max(processed1.length(), processed2.length()));
        }
        
        // Score
        Scorer scorer(kmpDensity, rkDensity, lcsScore, saScore);
        return scorer.getResult();
    }
};

TEST_F(IntegrationTest, ParaphrasedTextPlagiarism) {
    // Test sample1.txt vs sample2.txt (paraphrased content, ~30% similarity)
    ScorerResult result = runFullPipeline("data/sample1.txt", "data/sample2.txt", false);
    
    std::cout << "Paraphrased Text Test:\n";
    std::cout << "  KMP: " << result.kmpDensity << "\n";
    std::cout << "  RK: " << result.rkDensity << "\n";
    std::cout << "  LCS: " << result.lcsScore << "\n";
    std::cout << "  SA: " << result.saScore << "\n";
    std::cout << "  Final: " << result.finalScore << " - " << result.verdict << "\n\n";
    
    // Should score between 0.25-0.45 (Low similarity range)
    EXPECT_GE(result.finalScore, 0.25);
    EXPECT_LE(result.finalScore, 0.45);
    EXPECT_EQ(result.verdict, "Low similarity");
}

TEST_F(IntegrationTest, CodePlagiarismWithRenamedVariables) {
    // Test original vs plagiarised code (same algorithm, renamed variables)
    ScorerResult result = runFullPipeline("data/sample2_original.cpp", 
                                         "data/sample2_plagiarised.cpp", true);
    
    std::cout << "Code Plagiarism Test:\n";
    std::cout << "  KMP: " << result.kmpDensity << "\n";
    std::cout << "  RK: " << result.rkDensity << "\n";
    std::cout << "  LCS: " << result.lcsScore << "\n";
    std::cout << "  SA: " << result.saScore << "\n";
    std::cout << "  Final: " << result.finalScore << " - " << result.verdict << "\n\n";
    
    // Should score above 0.60 (Moderate to High plagiarism)
    EXPECT_GE(result.finalScore, 0.60);
    EXPECT_TRUE(result.verdict == "Moderate plagiarism" || 
                result.verdict == "High plagiarism");
}

TEST_F(IntegrationTest, CompletelyDifferentDocuments) {
    // Test completely different documents
    ScorerResult result = runFullPipeline("data/sample1.txt", 
                                         "data/sample2_original.cpp", false);
    
    std::cout << "Different Documents Test:\n";
    std::cout << "  KMP: " << result.kmpDensity << "\n";
    std::cout << "  RK: " << result.rkDensity << "\n";
    std::cout << "  LCS: " << result.lcsScore << "\n";
    std::cout << "  SA: " << result.saScore << "\n";
    std::cout << "  Final: " << result.finalScore << " - " << result.verdict << "\n\n";
    
    // Should score below 0.20 (Original)
    EXPECT_LT(result.finalScore, 0.20);
    EXPECT_EQ(result.verdict, "Original");
}

TEST_F(IntegrationTest, IdenticalDocuments) {
    // Test identical documents (should score very high)
    ScorerResult result = runFullPipeline("data/sample1.txt", 
                                         "data/sample1.txt", false);
    
    std::cout << "Identical Documents Test:\n";
    std::cout << "  KMP: " << result.kmpDensity << "\n";
    std::cout << "  RK: " << result.rkDensity << "\n";
    std::cout << "  LCS: " << result.lcsScore << "\n";
    std::cout << "  SA: " << result.saScore << "\n";
    std::cout << "  Final: " << result.finalScore << " - " << result.verdict << "\n\n";
    
    // Should score very high (near 1.0)
    EXPECT_GE(result.finalScore, 0.90);
    EXPECT_EQ(result.verdict, "High plagiarism");
}

TEST_F(IntegrationTest, CodeVsCodeDifferent) {
    // Test two different code samples
    ScorerResult result = runFullPipeline("data/sample_code1.cpp", 
                                         "data/sample_code2.cpp", true);
    
    std::cout << "Different Code Test:\n";
    std::cout << "  KMP: " << result.kmpDensity << "\n";
    std::cout << "  RK: " << result.rkDensity << "\n";
    std::cout << "  LCS: " << result.lcsScore << "\n";
    std::cout << "  SA: " << result.saScore << "\n";
    std::cout << "  Final: " << result.finalScore << " - " << result.verdict << "\n\n";
    
    // These are similar but not plagiarized (both have factorial and max functions)
    // Should be in low to moderate range
    EXPECT_GE(result.finalScore, 0.20);
    EXPECT_LT(result.finalScore, 0.70);
}

TEST_F(IntegrationTest, AlgorithmConsistency) {
    // Verify that all algorithms produce reasonable scores
    ScorerResult result = runFullPipeline("data/sample1.txt", 
                                         "data/sample2.txt", false);
    
    // All individual scores should be between 0 and 1
    EXPECT_GE(result.kmpDensity, 0.0);
    EXPECT_LE(result.kmpDensity, 1.0);
    
    EXPECT_GE(result.rkDensity, 0.0);
    EXPECT_LE(result.rkDensity, 1.0);
    
    EXPECT_GE(result.lcsScore, 0.0);
    EXPECT_LE(result.lcsScore, 1.0);
    
    EXPECT_GE(result.saScore, 0.0);
    EXPECT_LE(result.saScore, 1.0);
    
    // Final score should also be in valid range
    EXPECT_GE(result.finalScore, 0.0);
    EXPECT_LE(result.finalScore, 1.0);
}

TEST_F(IntegrationTest, VerdictCategories) {
    // Test that verdicts are assigned correctly
    
    // Low score -> Original
    Scorer scorer1(0.1, 0.1, 0.1, 0.1);
    EXPECT_EQ(scorer1.getResult().verdict, "Original");
    
    // Medium-low score -> Low similarity
    Scorer scorer2(0.3, 0.3, 0.3, 0.3);
    EXPECT_EQ(scorer2.getResult().verdict, "Low similarity");
    
    // Medium-high score -> Moderate plagiarism
    Scorer scorer3(0.6, 0.6, 0.6, 0.6);
    EXPECT_EQ(scorer3.getResult().verdict, "Moderate plagiarism");
    
    // High score -> High plagiarism
    Scorer scorer4(0.9, 0.9, 0.9, 0.9);
    EXPECT_EQ(scorer4.getResult().verdict, "High plagiarism");
}

TEST_F(IntegrationTest, PreprocessingEffectiveness) {
    // Test that preprocessing improves detection
    std::string text1 = "The QUICK brown FOX jumps!";
    std::string text2 = "the quick brown fox jumps";
    
    TextPreprocessor proc;
    std::string processed1 = proc.preprocess(text1, false);
    std::string processed2 = proc.preprocess(text2, false);
    
    LCS lcs;
    double score = lcs.similarityScore(processed1, processed2);
    
    // After preprocessing, these should be very similar
    EXPECT_GT(score, 0.8);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
