#ifndef PATTERN_MATCHER_H
#define PATTERN_MATCHER_H

#include <string>
#include <vector>
#include <map>

/**
 * @brief Abstract base class for pattern matching algorithms
 * 
 * Provides a common interface for different pattern matching implementations
 * such as KMP, Rabin-Karp, Boyer-Moore, etc.
 */
class PatternMatcher {
public:
    virtual ~PatternMatcher() = default;
    
    /**
     * @brief Search for pattern in text and return all match positions
     * @param text The text to search in
     * @param pattern The pattern to search for
     * @return Vector of starting positions where pattern is found
     */
    virtual std::vector<size_t> search(const std::string& text, const std::string& pattern) = 0;
    
    /**
     * @brief Calculate match density (coverage ratio)
     * @param text The text to search in
     * @param pattern The pattern to search for
     * @return Ratio of (matches * patternLen) / textLen
     */
    double matchDensity(const std::string& text, const std::string& pattern);
};

/**
 * @brief Knuth-Morris-Pratt pattern matching algorithm
 * 
 * Efficient string matching using failure function to avoid redundant comparisons.
 * Time Complexity: O(n + m) where n = text length, m = pattern length
 * Space Complexity: O(m) for failure function
 */
class KMP : public PatternMatcher {
public:
    /**
     * @brief Build the failure function (LPS array) for KMP algorithm
     * @param pattern The pattern to build failure function for
     * @return Failure function as vector of integers
     */
    std::vector<int> buildFailureFunction(const std::string& pattern);
    
    /**
     * @brief Search for pattern in text using KMP algorithm
     * @param text The text to search in
     * @param pattern The pattern to search for
     * @return Vector of starting positions where pattern is found
     */
    std::vector<size_t> search(const std::string& text, const std::string& pattern) override;
};

/**
 * @brief Rabin-Karp rolling hash pattern matching algorithm
 * 
 * Uses rolling hash for efficient pattern matching with multiple patterns.
 * Time Complexity: O(n + m) average case, O(nm) worst case
 * Space Complexity: O(1) for single pattern, O(k) for k patterns
 */
class RabinKarp : public PatternMatcher {
public:
    /**
     * @brief Constructor with default hash parameters
     * @param base Base for rolling hash (default: 31)
     * @param mod Modulo for hash values (default: 1e9+7)
     */
    RabinKarp(long long base = 31, long long mod = 1000000007);
    
    /**
     * @brief Search for single pattern in text using Rabin-Karp
     * @param text The text to search in
     * @param pattern The pattern to search for
     * @return Vector of starting positions where pattern is found
     */
    std::vector<size_t> search(const std::string& text, const std::string& pattern) override;
    
    /**
     * @brief Search for multiple patterns simultaneously
     * @param text The text to search in
     * @param patterns Vector of patterns to search for
     * @return Map from pattern to vector of positions where it's found
     */
    std::map<std::string, std::vector<size_t>> searchMultiple(
        const std::string& text, 
        const std::vector<std::string>& patterns);

private:
    long long base_;
    long long mod_;
    
    /**
     * @brief Calculate hash value for a string
     * @param str String to hash
     * @return Hash value
     */
    long long calculateHash(const std::string& str) const;
    
    /**
     * @brief Update rolling hash when sliding window
     * @param oldHash Previous hash value
     * @param oldChar Character leaving the window
     * @param newChar Character entering the window
     * @param patternLen Length of the pattern
     * @param highestPower base^(patternLen-1) mod mod_
     * @return New hash value
     */
    long long rollingHash(long long oldHash, char oldChar, char newChar, 
                         size_t patternLen, long long highestPower) const;
    
    /**
     * @brief Verify if pattern matches text at given position (handle collisions)
     * @param text The text
     * @param pattern The pattern
     * @param pos Position to verify
     * @return True if pattern matches at position
     */
    bool verifyMatch(const std::string& text, const std::string& pattern, size_t pos) const;
    
    /**
     * @brief Calculate base^exp mod mod_
     * @param exp Exponent
     * @return base^exp mod mod_
     */
    long long power(size_t exp) const;
};

#endif // PATTERN_MATCHER_H
