#include "preprocessor.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <regex>

// ============================================================================
// TextPreprocessor Implementation
// ============================================================================

TextPreprocessor::TextPreprocessor() {
    // Initialize common English stopwords
    stopwords_ = {
        "a", "an", "and", "are", "as", "at", "be", "by", "for", "from",
        "has", "he", "in", "is", "it", "its", "of", "on", "that", "the",
        "to", "was", "will", "with", "the", "this", "but", "they", "have",
        "had", "what", "when", "where", "who", "which", "why", "how"
    };
}

std::vector<std::string> TextPreprocessor::tokenize(const std::string& text) const {
    std::vector<std::string> tokens;
    std::string token;
    
    for (char c : text) {
        if (std::isalnum(c) || c == '\'') {
            token += c;
        } else if (!token.empty()) {
            tokens.push_back(token);
            token.clear();
        }
    }
    
    if (!token.empty()) {
        tokens.push_back(token);
    }
    
    return tokens;
}

std::vector<std::string> TextPreprocessor::removeStopwords(
    const std::vector<std::string>& tokens) const {
    std::vector<std::string> filtered;
    
    for (const auto& token : tokens) {
        std::string lower = token;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        
        if (stopwords_.find(lower) == stopwords_.end()) {
            filtered.push_back(token);
        }
    }
    
    return filtered;
}

std::string TextPreprocessor::normalize(const std::string& text) const {
    std::string result;
    
    for (char c : text) {
        if (std::isalnum(c) || std::isspace(c)) {
            result += std::tolower(c);
        } else if (!result.empty() && result.back() != ' ') {
            result += ' ';
        }
    }
    
    return result;
}

// Basic Porter Stemmer Implementation
bool TextPreprocessor::isConsonant(const std::string& word, size_t pos) const {
    if (pos >= word.length()) return false;
    
    char c = std::tolower(word[pos]);
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
        return false;
    }
    if (c == 'y') {
        return pos == 0 ? true : !isConsonant(word, pos - 1);
    }
    return true;
}

int TextPreprocessor::measureWord(const std::string& word) const {
    int measure = 0;
    bool consonantSeen = false;
    
    for (size_t i = 0; i < word.length(); ++i) {
        if (isConsonant(word, i)) {
            consonantSeen = true;
        } else if (consonantSeen) {
            measure++;
            consonantSeen = false;
        }
    }
    
    return measure;
}

bool TextPreprocessor::containsVowel(const std::string& word) const {
    for (size_t i = 0; i < word.length(); ++i) {
        if (!isConsonant(word, i)) {
            return true;
        }
    }
    return false;
}

bool TextPreprocessor::endsWithDoubleConsonant(const std::string& word) const {
    if (word.length() < 2) return false;
    return word[word.length() - 1] == word[word.length() - 2] &&
           isConsonant(word, word.length() - 1);
}

bool TextPreprocessor::endsWithCVC(const std::string& word) const {
    if (word.length() < 3) return false;
    
    size_t len = word.length();
    char last = std::tolower(word[len - 1]);
    
    return isConsonant(word, len - 3) &&
           !isConsonant(word, len - 2) &&
           isConsonant(word, len - 1) &&
           last != 'w' && last != 'x' && last != 'y';
}

std::string TextPreprocessor::step1a(const std::string& word) const {
    std::string result = word;
    
    if (result.length() > 4 && result.substr(result.length() - 4) == "sses") {
        result = result.substr(0, result.length() - 2);
    } else if (result.length() > 3 && result.substr(result.length() - 3) == "ies") {
        result = result.substr(0, result.length() - 2);
    } else if (result.length() > 2 && result.substr(result.length() - 2) == "ss") {
        // Keep as is
    } else if (result.length() > 1 && result.back() == 's') {
        result = result.substr(0, result.length() - 1);
    }
    
    return result;
}

std::string TextPreprocessor::step1b(const std::string& word) const {
    std::string result = word;
    
    if (result.length() > 3 && result.substr(result.length() - 3) == "eed") {
        if (measureWord(result.substr(0, result.length() - 3)) > 0) {
            result = result.substr(0, result.length() - 1);
        }
    } else if (result.length() > 2 && result.substr(result.length() - 2) == "ed") {
        std::string stem = result.substr(0, result.length() - 2);
        if (containsVowel(stem)) {
            result = stem;
        }
    } else if (result.length() > 3 && result.substr(result.length() - 3) == "ing") {
        std::string stem = result.substr(0, result.length() - 3);
        if (containsVowel(stem)) {
            result = stem;
        }
    }
    
    return result;
}

std::string TextPreprocessor::step1c(const std::string& word) const {
    std::string result = word;
    
    if (result.length() > 1 && result.back() == 'y' && containsVowel(result.substr(0, result.length() - 1))) {
        result[result.length() - 1] = 'i';
    }
    
    return result;
}

std::string TextPreprocessor::step2(const std::string& word) const {
    std::string result = word;
    
    if (result.length() > 7 && result.substr(result.length() - 7) == "ational") {
        if (measureWord(result.substr(0, result.length() - 7)) > 0) {
            result = result.substr(0, result.length() - 5) + "e";
        }
    } else if (result.length() > 6 && result.substr(result.length() - 6) == "tional") {
        if (measureWord(result.substr(0, result.length() - 6)) > 0) {
            result = result.substr(0, result.length() - 2);
        }
    }
    
    return result;
}

std::string TextPreprocessor::stemWord(const std::string& word) const {
    if (word.length() <= 2) {
        return word;
    }
    
    std::string result = word;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    
    // Apply Porter stemmer steps
    result = step1a(result);
    result = step1b(result);
    result = step1c(result);
    result = step2(result);
    
    return result;
}

std::string TextPreprocessor::preprocess(const std::string& input, bool isCode) {
    // Normalize text
    std::string normalized = normalize(input);
    
    // Tokenize
    std::vector<std::string> tokens = tokenize(normalized);
    
    // Remove stopwords
    tokens = removeStopwords(tokens);
    
    // Stem each token
    std::vector<std::string> stemmed;
    for (const auto& token : tokens) {
        stemmed.push_back(stemWord(token));
    }
    
    // Join tokens back into string
    std::ostringstream oss;
    for (size_t i = 0; i < stemmed.size(); ++i) {
        if (i > 0) oss << " ";
        oss << stemmed[i];
    }
    
    return oss.str();
}

// ============================================================================
// CodePreprocessor Implementation
// ============================================================================

CodePreprocessor::CodePreprocessor() {
    // Initialize common programming keywords (C/C++/Java style)
    keywords_ = {
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while",
        "class", "namespace", "template", "public", "private", "protected", "virtual",
        "bool", "true", "false", "nullptr", "new", "delete", "this", "try", "catch",
        "throw", "using", "inline", "explicit", "operator", "friend", "const_cast",
        "dynamic_cast", "reinterpret_cast", "static_cast"
    };
}

std::string CodePreprocessor::stripComments(const std::string& code) const {
    std::string result;
    bool inSingleLineComment = false;
    bool inMultiLineComment = false;
    bool inString = false;
    bool inChar = false;
    
    for (size_t i = 0; i < code.length(); ++i) {
        char current = code[i];
        char next = (i + 1 < code.length()) ? code[i + 1] : '\0';
        
        // Handle string literals
        if (current == '"' && !inChar && !inSingleLineComment && !inMultiLineComment) {
            if (i == 0 || code[i - 1] != '\\') {
                inString = !inString;
            }
            result += current;
            continue;
        }
        
        // Handle char literals
        if (current == '\'' && !inString && !inSingleLineComment && !inMultiLineComment) {
            if (i == 0 || code[i - 1] != '\\') {
                inChar = !inChar;
            }
            result += current;
            continue;
        }
        
        // Skip processing if inside string or char literal
        if (inString || inChar) {
            result += current;
            continue;
        }
        
        // Handle single-line comments
        if (current == '/' && next == '/' && !inMultiLineComment) {
            inSingleLineComment = true;
            ++i;
            continue;
        }
        
        // Handle multi-line comment start
        if (current == '/' && next == '*' && !inSingleLineComment) {
            inMultiLineComment = true;
            ++i;
            continue;
        }
        
        // Handle multi-line comment end
        if (current == '*' && next == '/' && inMultiLineComment) {
            inMultiLineComment = false;
            ++i;
            continue;
        }
        
        // Handle newline (ends single-line comment)
        if (current == '\n') {
            inSingleLineComment = false;
            result += current;
            continue;
        }
        
        // Add character if not in comment
        if (!inSingleLineComment && !inMultiLineComment) {
            result += current;
        }
    }
    
    return result;
}

bool CodePreprocessor::isKeyword(const std::string& token) const {
    return keywords_.find(token) != keywords_.end();
}

bool CodePreprocessor::isOperator(char c) const {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
           c == '=' || c == '<' || c == '>' || c == '!' || c == '&' ||
           c == '|' || c == '^' || c == '~' || c == '?';
}

bool CodePreprocessor::isDelimiter(char c) const {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' ||
           c == ';' || c == ',' || c == '.' || c == ':';
}

std::vector<std::string> CodePreprocessor::tokenizeCode(const std::string& code) const {
    std::vector<std::string> tokens;
    std::string token;
    
    for (size_t i = 0; i < code.length(); ++i) {
        char c = code[i];
        
        if (std::isalnum(c) || c == '_') {
            token += c;
        } else {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            
            if (isOperator(c) || isDelimiter(c)) {
                tokens.push_back(std::string(1, c));
            }
        }
    }
    
    if (!token.empty()) {
        tokens.push_back(token);
    }
    
    return tokens;
}

std::string CodePreprocessor::normalizeIdentifiers(const std::string& code) const {
    std::vector<std::string> tokens = tokenizeCode(code);
    std::unordered_map<std::string, std::string> identifierMap;
    int varCounter = 0;
    
    std::ostringstream result;
    
    for (const auto& token : tokens) {
        // Check if token is an identifier (not keyword, not number, not operator/delimiter)
        if (!token.empty() && (std::isalpha(token[0]) || token[0] == '_')) {
            if (!isKeyword(token)) {
                // This is a user-defined identifier
                if (identifierMap.find(token) == identifierMap.end()) {
                    identifierMap[token] = "VAR_" + std::to_string(varCounter++);
                }
                result << identifierMap[token] << " ";
            } else {
                // Keep keywords as-is
                result << token << " ";
            }
        } else {
            // Keep operators, delimiters, and numbers as-is
            result << token << " ";
        }
    }
    
    std::string output = result.str();
    if (!output.empty() && output.back() == ' ') {
        output.pop_back();
    }
    
    return output;
}

std::string CodePreprocessor::preprocess(const std::string& input, bool isCode) {
    // Strip comments
    std::string noComments = stripComments(input);
    
    // Normalize identifiers
    std::string normalized = normalizeIdentifiers(noComments);
    
    return normalized;
}
