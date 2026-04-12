#include "pattern_matcher.h"
#include <unordered_map>

// ============================================================================
// Rabin-Karp Algorithm Implementation
// ============================================================================

RabinKarp::RabinKarp(long long base, long long mod) 
    : base_(base), mod_(mod) {
}

long long RabinKarp::calculateHash(const std::string& str) const {
    long long hash = 0;
    long long pow = 1;
    
    for (size_t i = 0; i < str.length(); ++i) {
        hash = (hash + (str[i] * pow) % mod_) % mod_;
        pow = (pow * base_) % mod_;
    }
    
    return hash;
}

long long RabinKarp::power(size_t exp) const {
    long long result = 1;
    long long base = base_;
    
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod_;
        }
        base = (base * base) % mod_;
        exp /= 2;
    }
    
    return result;
}

long long RabinKarp::rollingHash(long long oldHash, char oldChar, char newChar, 
                                 size_t patternLen, long long highestPower) const {
    // Remove contribution of old character
    oldHash = (oldHash - oldChar + mod_) % mod_;
    
    // Divide by base (shift right)
    oldHash = (oldHash * power(mod_ - 2)) % mod_; // Modular multiplicative inverse
    
    // Add contribution of new character at the end
    long long newHash = (oldHash + (newChar * highestPower) % mod_) % mod_;
    
    return newHash;
}

bool RabinKarp::verifyMatch(const std::string& text, const std::string& pattern, size_t pos) const {
    if (pos + pattern.length() > text.length()) {
        return false;
    }
    
    for (size_t i = 0; i < pattern.length(); ++i) {
        if (text[pos + i] != pattern[i]) {
            return false;
        }
    }
    
    return true;
}

std::vector<size_t> RabinKarp::search(const std::string& text, const std::string& pattern) {
    std::vector<size_t> positions;
    
    if (pattern.empty() || text.empty() || pattern.length() > text.length()) {
        return positions;
    }
    
    size_t n = text.length();
    size_t m = pattern.length();
    
    // Calculate hash of pattern
    long long patternHash = calculateHash(pattern);
    
    // Calculate hash of first window of text
    std::string firstWindow = text.substr(0, m);
    long long textHash = calculateHash(firstWindow);
    
    // Calculate base^(m-1) % mod for rolling hash
    long long highestPower = power(m - 1);
    
    // Check first window
    if (textHash == patternHash && verifyMatch(text, pattern, 0)) {
        positions.push_back(0);
    }
    
    // Slide the window over text
    for (size_t i = 1; i <= n - m; ++i) {
        // Calculate hash for current window using rolling hash
        textHash = (textHash - text[i - 1] + mod_) % mod_;
        textHash = (textHash * power(mod_ - 2)) % mod_; // Divide by base
        textHash = (textHash + (text[i + m - 1] * highestPower) % mod_) % mod_;
        
        // If hash matches, verify the actual string to handle collisions
        if (textHash == patternHash && verifyMatch(text, pattern, i)) {
            positions.push_back(i);
        }
    }
    
    return positions;
}

std::map<std::string, std::vector<size_t>> RabinKarp::searchMultiple(
    const std::string& text, 
    const std::vector<std::string>& patterns) {
    
    std::map<std::string, std::vector<size_t>> results;
    
    if (text.empty() || patterns.empty()) {
        return results;
    }
    
    // Group patterns by length for efficiency
    std::unordered_map<size_t, std::vector<std::string>> patternsByLength;
    for (const auto& pattern : patterns) {
        if (!pattern.empty() && pattern.length() <= text.length()) {
            patternsByLength[pattern.length()].push_back(pattern);
        }
    }
    
    // Process each length group
    for (const auto& [length, patternsOfLength] : patternsByLength) {
        size_t m = length;
        size_t n = text.length();
        
        // Build hash map for all patterns of this length
        std::unordered_map<long long, std::vector<std::string>> patternHashMap;
        for (const auto& pattern : patternsOfLength) {
            long long hash = calculateHash(pattern);
            patternHashMap[hash].push_back(pattern);
        }
        
        // Calculate hash of first window
        std::string firstWindow = text.substr(0, m);
        long long textHash = calculateHash(firstWindow);
        long long highestPower = power(m - 1);
        
        // Check first window
        if (patternHashMap.find(textHash) != patternHashMap.end()) {
            for (const auto& pattern : patternHashMap[textHash]) {
                if (verifyMatch(text, pattern, 0)) {
                    results[pattern].push_back(0);
                }
            }
        }
        
        // Slide window over text
        for (size_t i = 1; i <= n - m; ++i) {
            // Rolling hash
            textHash = (textHash - text[i - 1] + mod_) % mod_;
            textHash = (textHash * power(mod_ - 2)) % mod_;
            textHash = (textHash + (text[i + m - 1] * highestPower) % mod_) % mod_;
            
            // Check if any pattern matches this hash
            if (patternHashMap.find(textHash) != patternHashMap.end()) {
                for (const auto& pattern : patternHashMap[textHash]) {
                    if (verifyMatch(text, pattern, i)) {
                        results[pattern].push_back(i);
                    }
                }
            }
        }
    }
    
    return results;
}
