#include "scorer.h"
#include <sstream>
#include <fstream>
#include <iomanip>

std::string ReportGenerator::escapeJSON(const std::string& str) const {
    std::ostringstream oss;
    for (char c : str) {
        switch (c) {
            case '"':  oss << "\\\""; break;
            case '\\': oss << "\\\\"; break;
            case '\b': oss << "\\b"; break;
            case '\f': oss << "\\f"; break;
            case '\n': oss << "\\n"; break;
            case '\r': oss << "\\r"; break;
            case '\t': oss << "\\t"; break;
            default:
                if (c < 32) {
                    oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
                } else {
                    oss << c;
                }
        }
    }
    return oss.str();
}

std::string ReportGenerator::doubleToString(double value, int precision) const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

std::string ReportGenerator::toJSON(const ScorerResult& result,
                                   const std::vector<std::pair<int, int>>& spans,
                                   const std::string& doc1,
                                   const std::string& doc2) const {
    std::ostringstream json;
    
    json << "{\n";
    json << "  \"score\": " << doubleToString(result.finalScore) << ",\n";
    json << "  \"verdict\": \"" << escapeJSON(result.verdict) << "\",\n";
    
    json << "  \"algorithm_breakdown\": {\n";
    json << "    \"kmp_density\": " << doubleToString(result.kmpDensity) << ",\n";
    json << "    \"rabin_karp_density\": " << doubleToString(result.rkDensity) << ",\n";
    json << "    \"lcs_score\": " << doubleToString(result.lcsScore) << ",\n";
    json << "    \"suffix_array_score\": " << doubleToString(result.saScore) << "\n";
    json << "  },\n";
    
    json << "  \"matched_regions\": [\n";
    for (size_t i = 0; i < spans.size(); ++i) {
        json << "    {\n";
        json << "      \"start\": " << spans[i].first << ",\n";
        json << "      \"end\": " << spans[i].second << ",\n";
        
        int start = spans[i].first;
        int end = spans[i].second;
        if (start >= 0 && end <= static_cast<int>(doc1.length()) && start < end) {
            std::string excerpt = doc1.substr(start, end - start);
            if (excerpt.length() > 100) {
                excerpt = excerpt.substr(0, 97) + "...";
            }
            json << "      \"text\": \"" << escapeJSON(excerpt) << "\"\n";
        } else {
            json << "      \"text\": \"\"\n";
        }
        
        json << "    }";
        if (i < spans.size() - 1) json << ",";
        json << "\n";
    }
    json << "  ],\n";
    
    json << "  \"documents\": {\n";
    json << "    \"doc1_length\": " << doc1.length() << ",\n";
    json << "    \"doc2_length\": " << doc2.length() << "\n";
    json << "  }\n";
    
    json << "}\n";
    
    return json.str();
}

std::string ReportGenerator::toText(const ScorerResult& result,
                                   const std::vector<std::pair<int, int>>& spans,
                                   const std::string& doc1,
                                   const std::string& doc2) const {
    std::ostringstream text;
    
    text << "========================================\n";
    text << "   PLAGIARISM DETECTION REPORT\n";
    text << "========================================\n\n";
    
    text << "OVERALL ASSESSMENT\n";
    text << "------------------\n";
    text << "Final Score: " << doubleToString(result.finalScore, 2) << " / 1.00\n";
    text << "Verdict: " << result.verdict << "\n\n";
    
    text << "ALGORITHM BREAKDOWN\n";
    text << "-------------------\n";
    text << "KMP Density:        " << doubleToString(result.kmpDensity, 4) << " (weight: 0.20)\n";
    text << "Rabin-Karp Density: " << doubleToString(result.rkDensity, 4) << " (weight: 0.20)\n";
    text << "LCS Score:          " << doubleToString(result.lcsScore, 4) << " (weight: 0.35)\n";
    text << "Suffix Array Score: " << doubleToString(result.saScore, 4) << " (weight: 0.25)\n\n";
    
    text << "DOCUMENT STATISTICS\n";
    text << "-------------------\n";
    text << "Document 1 Length: " << doc1.length() << " characters\n";
    text << "Document 2 Length: " << doc2.length() << " characters\n";
    text << "Matched Regions:   " << spans.size() << "\n\n";
    
    if (!spans.empty()) {
        text << "MATCHED REGIONS\n";
        text << "---------------\n";
        
        for (size_t i = 0; i < spans.size(); ++i) {
            text << "Region " << (i + 1) << ": [" << spans[i].first << ", " << spans[i].second << ")\n";
            
            int start = spans[i].first;
            int end = spans[i].second;
            if (start >= 0 && end <= static_cast<int>(doc1.length()) && start < end) {
                std::string excerpt = doc1.substr(start, end - start);
                if (excerpt.length() > 80) {
                    excerpt = excerpt.substr(0, 77) + "...";
                }
                text << "  \"" << excerpt << "\"\n";
            }
            text << "\n";
        }
    }
    
    text << "========================================\n";
    text << "End of Report\n";
    text << "========================================\n";
    
    return text.str();
}

bool ReportGenerator::saveReport(const std::string& content, const std::string& filepath) const {
    std::ofstream file(filepath);
    
    if (!file.is_open()) {
        return false;
    }
    
    file << content;
    file.close();
    
    return true;
}
