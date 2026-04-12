#include "scorer.h"
#include <gtest/gtest.h>

TEST(ScorerTest, FinalScoreCalculation) {
    Scorer scorer(0.5, 0.5, 0.5, 0.5);
    EXPECT_DOUBLE_EQ(scorer.finalScore(), 0.5);
}

TEST(ScorerTest, VerdictOriginal) {
    Scorer scorer(0.1, 0.1, 0.1, 0.1);
    auto result = scorer.getResult();
    EXPECT_LT(result.finalScore, 0.20);
    EXPECT_EQ(result.verdict, "Original");
}

TEST(ScorerTest, VerdictLowSimilarity) {
    Scorer scorer(0.3, 0.3, 0.3, 0.3);
    auto result = scorer.getResult();
    EXPECT_GE(result.finalScore, 0.20);
    EXPECT_LT(result.finalScore, 0.45);
    EXPECT_EQ(result.verdict, "Low similarity");
}

TEST(ScorerTest, VerdictModeratePlagiarism) {
    Scorer scorer(0.6, 0.6, 0.6, 0.6);
    auto result = scorer.getResult();
    EXPECT_GE(result.finalScore, 0.45);
    EXPECT_LT(result.finalScore, 0.70);
    EXPECT_EQ(result.verdict, "Moderate plagiarism");
}

TEST(ScorerTest, VerdictHighPlagiarism) {
    Scorer scorer(0.9, 0.9, 0.9, 0.9);
    auto result = scorer.getResult();
    EXPECT_GE(result.finalScore, 0.70);
    EXPECT_EQ(result.verdict, "High plagiarism");
}

TEST(ScorerTest, WeightedScoring) {
    Scorer scorer(1.0, 0.0, 0.0, 0.0);
    double score = scorer.finalScore();
    EXPECT_DOUBLE_EQ(score, 0.20);
    
    Scorer scorer2(0.0, 1.0, 0.0, 0.0);
    EXPECT_DOUBLE_EQ(scorer2.finalScore(), 0.20);
    
    Scorer scorer3(0.0, 0.0, 1.0, 0.0);
    EXPECT_DOUBLE_EQ(scorer3.finalScore(), 0.35);
    
    Scorer scorer4(0.0, 0.0, 0.0, 1.0);
    EXPECT_DOUBLE_EQ(scorer4.finalScore(), 0.25);
}

TEST(ReportGeneratorTest, JSONGeneration) {
    ReportGenerator gen;
    ScorerResult result;
    result.kmpDensity = 0.5;
    result.rkDensity = 0.4;
    result.lcsScore = 0.6;
    result.saScore = 0.5;
    result.finalScore = 0.525;
    result.verdict = "Moderate plagiarism";
    
    std::vector<std::pair<int, int>> spans = {{0, 5}, {10, 15}};
    std::string doc1 = "hello world test";
    std::string doc2 = "hello universe";
    
    std::string json = gen.toJSON(result, spans, doc1, doc2);
    
    EXPECT_NE(json.find("\"score\""), std::string::npos);
    EXPECT_NE(json.find("\"verdict\""), std::string::npos);
    EXPECT_NE(json.find("\"algorithm_breakdown\""), std::string::npos);
    EXPECT_NE(json.find("\"matched_regions\""), std::string::npos);
    EXPECT_NE(json.find("0.5250"), std::string::npos);
}

TEST(ReportGeneratorTest, TextGeneration) {
    ReportGenerator gen;
    ScorerResult result;
    result.kmpDensity = 0.3;
    result.rkDensity = 0.3;
    result.lcsScore = 0.4;
    result.saScore = 0.3;
    result.finalScore = 0.34;
    result.verdict = "Low similarity";
    
    std::vector<std::pair<int, int>> spans = {{0, 5}};
    std::string doc1 = "hello world";
    std::string doc2 = "hello test";
    
    std::string text = gen.toText(result, spans, doc1, doc2);
    
    EXPECT_NE(text.find("PLAGIARISM DETECTION REPORT"), std::string::npos);
    EXPECT_NE(text.find("Final Score"), std::string::npos);
    EXPECT_NE(text.find("Low similarity"), std::string::npos);
    EXPECT_NE(text.find("ALGORITHM BREAKDOWN"), std::string::npos);
}

TEST(ReportGeneratorTest, JSONEscaping) {
    ReportGenerator gen;
    ScorerResult result;
    result.finalScore = 0.5;
    result.verdict = "Test \"quoted\" text";
    result.kmpDensity = 0.5;
    result.rkDensity = 0.5;
    result.lcsScore = 0.5;
    result.saScore = 0.5;
    
    std::vector<std::pair<int, int>> spans;
    std::string doc1 = "Line 1\nLine 2\tTabbed";
    std::string doc2 = "test";
    
    std::string json = gen.toJSON(result, spans, doc1, doc2);
    
    EXPECT_NE(json.find("\\\"quoted\\\""), std::string::npos);
}

TEST(ReportGeneratorTest, SaveReport) {
    ReportGenerator gen;
    std::string content = "Test report content";
    std::string filepath = "test_report.txt";
    
    bool success = gen.saveReport(content, filepath);
    EXPECT_TRUE(success);
}

TEST(ReportGeneratorTest, EmptySpans) {
    ReportGenerator gen;
    ScorerResult result;
    result.finalScore = 0.1;
    result.verdict = "Original";
    result.kmpDensity = 0.1;
    result.rkDensity = 0.1;
    result.lcsScore = 0.1;
    result.saScore = 0.1;
    
    std::vector<std::pair<int, int>> spans;
    std::string doc1 = "test";
    std::string doc2 = "test";
    
    std::string json = gen.toJSON(result, spans, doc1, doc2);
    EXPECT_NE(json.find("\"matched_regions\": []"), std::string::npos);
    
    std::string text = gen.toText(result, spans, doc1, doc2);
    EXPECT_NE(text.find("Matched Regions:   0"), std::string::npos);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
