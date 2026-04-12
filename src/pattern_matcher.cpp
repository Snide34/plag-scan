#include "pattern_matcher.h"
#include <algorithm>

// ============================================================================
// PatternMatcher Base Class Implementation
// ============================================================================

double PatternMatcher::matchDensity(const std::string& text, const std::string& pattern) {
    if (text.empty() || pattern.empty()) {
        return 0.0;
    }
    
    std::vector<size_t> matches = search(text, pattern);
    
    if (matches.empty()) {
        return 0.0;
    }
    
    // Calculate density: (number of matches * pattern length) / text length
    double numerator = static_cast<double>(matches.size() * pattern.length());
    double denominator = static_cast<double>(text.length());
    
    return numerator / denominator;
}
