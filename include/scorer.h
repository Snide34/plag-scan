#ifndef SCORER_H
#define SCORER_H

#include <string>
#include <vector>
#include <utility>

struct ScorerResult {
    double kmpDensity;
    double rkDensity;
    double lcsScore;
    double saScore;
    double finalScore;
    std::string verdict;
};

class Scorer {
public:
    Scorer(double kmpDensity = 0.0, double rkDensity = 0.0, 
           double lcsScore = 0.0, double saScore = 0.0);
    
    double finalScore() const;
    std::string verdict(double score) const;
    ScorerResult getResult() const;

private:
    double kmpDensity_;
    double rkDensity_;
    double lcsScore_;
    double saScore_;
    
    static constexpr double KMP_WEIGHT = 0.20;
    static constexpr double RK_WEIGHT = 0.20;
    static constexpr double LCS_WEIGHT = 0.35;
    static constexpr double SA_WEIGHT = 0.25;
};

class ReportGenerator {
public:
    std::string toJSON(const ScorerResult& result, 
                      const std::vector<std::pair<int, int>>& spans,
                      const std::string& doc1, 
                      const std::string& doc2) const;
    
    std::string toText(const ScorerResult& result,
                      const std::vector<std::pair<int, int>>& spans,
                      const std::string& doc1,
                      const std::string& doc2) const;
    
    bool saveReport(const std::string& content, const std::string& filepath) const;

private:
    std::string escapeJSON(const std::string& str) const;
    std::string doubleToString(double value, int precision = 4) const;
};

#endif // SCORER_H
