#include "preprocessor.h"
#include <gtest/gtest.h>

// ============================================================================
// TextPreprocessor Tests
// ============================================================================

TEST(TextPreprocessorTest, Tokenize) {
    TextPreprocessor proc;
    std::string input = "Hello, world! This is a test.";
    auto tokens = proc.tokenize(input);
    
    ASSERT_EQ(tokens.size(), 6);
    EXPECT_EQ(tokens[0], "Hello");
    EXPECT_EQ(tokens[1], "world");
    EXPECT_EQ(tokens[2], "This");
}

TEST(TextPreprocessorTest, RemoveStopwords) {
    TextPreprocessor proc;
    std::vector<std::string> tokens = {"this", "is", "a", "test", "with", "words"};
    auto filtered = proc.removeStopwords(tokens);
    
    EXPECT_LT(filtered.size(), tokens.size());
    EXPECT_NE(std::find(filtered.begin(), filtered.end(), "test"), filtered.end());
    EXPECT_NE(std::find(filtered.begin(), filtered.end(), "words"), filtered.end());
}

TEST(TextPreprocessorTest, Normalize) {
    TextPreprocessor proc;
    std::string input = "Hello, WORLD! 123";
    std::string normalized = proc.normalize(input);
    
    EXPECT_EQ(normalized, "hello world 123");
}

TEST(TextPreprocessorTest, StemWord) {
    TextPreprocessor proc;
    
    EXPECT_EQ(proc.stemWord("running"), "run");
    EXPECT_EQ(proc.stemWord("flies"), "fli");
    EXPECT_EQ(proc.stemWord("national"), "nation");
}

TEST(TextPreprocessorTest, Preprocess) {
    TextPreprocessor proc;
    std::string input = "The running dogs are playing in the park.";
    std::string result = proc.preprocess(input, false);
    
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.find("the"), std::string::npos); // Stopwords removed
}

// ============================================================================
// CodePreprocessor Tests
// ============================================================================

TEST(CodePreprocessorTest, StripSingleLineComments) {
    CodePreprocessor proc;
    std::string code = "int x = 5; // This is a comment\nint y = 10;";
    std::string result = proc.stripComments(code);
    
    EXPECT_EQ(result.find("//"), std::string::npos);
    EXPECT_NE(result.find("int x = 5;"), std::string::npos);
}

TEST(CodePreprocessorTest, StripMultiLineComments) {
    CodePreprocessor proc;
    std::string code = "int x = 5; /* This is a\nmulti-line comment */ int y = 10;";
    std::string result = proc.stripComments(code);
    
    EXPECT_EQ(result.find("/*"), std::string::npos);
    EXPECT_EQ(result.find("*/"), std::string::npos);
    EXPECT_NE(result.find("int x = 5;"), std::string::npos);
}

TEST(CodePreprocessorTest, TokenizeCode) {
    CodePreprocessor proc;
    std::string code = "int x = 42;";
    auto tokens = proc.tokenizeCode(code);
    
    EXPECT_GT(tokens.size(), 0);
    EXPECT_NE(std::find(tokens.begin(), tokens.end(), "int"), tokens.end());
    EXPECT_NE(std::find(tokens.begin(), tokens.end(), "x"), tokens.end());
    EXPECT_NE(std::find(tokens.begin(), tokens.end(), "42"), tokens.end());
}

TEST(CodePreprocessorTest, NormalizeIdentifiers) {
    CodePreprocessor proc;
    std::string code = "int myVar = 42; int anotherVar = myVar;";
    std::string result = proc.normalizeIdentifiers(code);
    
    EXPECT_NE(result.find("VAR_0"), std::string::npos);
    EXPECT_NE(result.find("VAR_1"), std::string::npos);
    EXPECT_NE(result.find("int"), std::string::npos); // Keywords preserved
}

TEST(CodePreprocessorTest, Preprocess) {
    CodePreprocessor proc;
    std::string code = "int myVar = 42; // comment\nint x = myVar * 2;";
    std::string result = proc.preprocess(code, true);
    
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.find("//"), std::string::npos); // Comments removed
    EXPECT_NE(result.find("VAR_"), std::string::npos); // Identifiers normalized
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
