#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

/**
 * @brief Base preprocessor interface
 */
class Preprocessor {
public:
    virtual ~Preprocessor() = default;
    virtual std::string preprocess(const std::string& input, bool isCode) = 0;
};

/**
 * @brief Text preprocessor for natural language processing
 * 
 * Provides tokenization, stopword removal, stemming, and normalization
 * for text documents to facilitate plagiarism detection.
 */
class TextPreprocessor : public Preprocessor {
public:
    TextPreprocessor();
    
    /**
     * @brief Tokenize input string into words
     * @param text Input text to tokenize
     * @return Vector of tokens
     */
    std::vector<std::string> tokenize(const std::string& text) const;
    
    /**
     * @brief Remove common stopwords from token list
     * @param tokens Input tokens
     * @return Filtered tokens without stopwords
     */
    std::vector<std::string> removeStopwords(const std::vector<std::string>& tokens) const;
    
    /**
     * @brief Apply basic Porter stemmer to reduce word to root form
     * @param word Input word
     * @return Stemmed word
     */
    std::string stemWord(const std::string& word) const;
    
    /**
     * @brief Normalize text: lowercase and remove punctuation
     * @param text Input text
     * @return Normalized text
     */
    std::string normalize(const std::string& text) const;
    
    /**
     * @brief Complete preprocessing pipeline for text
     * @param input Input text
     * @param isCode Ignored for TextPreprocessor
     * @return Preprocessed text as space-separated tokens
     */
    std::string preprocess(const std::string& input, bool isCode = false) override;

private:
    std::unordered_set<std::string> stopwords_;
    
    // Helper methods for Porter stemmer
    bool isConsonant(const std::string& word, size_t pos) const;
    int measureWord(const std::string& word) const;
    bool containsVowel(const std::string& word) const;
    bool endsWithDoubleConsonant(const std::string& word) const;
    bool endsWithCVC(const std::string& word) const;
    std::string step1a(const std::string& word) const;
    std::string step1b(const std::string& word) const;
    std::string step1c(const std::string& word) const;
    std::string step2(const std::string& word) const;
};

/**
 * @brief Code preprocessor for source code analysis
 * 
 * Provides comment removal, code tokenization, and identifier normalization
 * to enable code similarity detection and plagiarism checking.
 */
class CodePreprocessor : public Preprocessor {
public:
    CodePreprocessor();
    
    /**
     * @brief Remove single-line and multi-line comments from code
     * @param code Input source code
     * @return Code with comments removed
     */
    std::string stripComments(const std::string& code) const;
    
    /**
     * @brief Tokenize code into meaningful units
     * @param code Input source code
     * @return Vector of code tokens
     */
    std::vector<std::string> tokenizeCode(const std::string& code) const;
    
    /**
     * @brief Replace variable names with generic placeholders
     * @param code Input source code
     * @return Code with normalized identifiers (VAR_0, VAR_1, etc.)
     */
    std::string normalizeIdentifiers(const std::string& code) const;
    
    /**
     * @brief Complete preprocessing pipeline for code
     * @param input Input source code
     * @param isCode Should be true for code preprocessing
     * @return Preprocessed code
     */
    std::string preprocess(const std::string& input, bool isCode = true) override;

private:
    std::unordered_set<std::string> keywords_;
    
    bool isKeyword(const std::string& token) const;
    bool isOperator(char c) const;
    bool isDelimiter(char c) const;
};

#endif // PREPROCESSOR_H
