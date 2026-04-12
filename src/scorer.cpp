#include "scorer.h"
#include <algorithm>

Scorer::Scorer(double kmpDensity, double rkDensity, double lcsScore, double saScore)
    : kmpDensity_(kmpDensity), rkDensity_(rkDensity), 
      lcsScore_(lcsScore), saScore_(saScore) {
}

double Scorer::finalScore() const {
    double score = KMP_WEIGHT * kmpDensity_ +
                   RK_WEIGHT * rkDensity_ +
                   LCS_WEIGHT * lcsScore_ +
                   SA_WEIGHT * saScore_;
    
    return std::min(1.0, std::max(0.0, score));
}

std::string Scorer::verdict(double score) const {
    if (score < 0.20) {
        return "Original";
    } else if (score < 0.45) {
        return "Low similarity";
    } else if (score < 0.70) {
        return "Moderate plagiarism";
    } else {
        return "High plagiarism";
    }
}

ScorerResult Scorer::getResult() const {
    ScorerResult result;
    result.kmpDensity = kmpDensity_;
    result.rkDensity = rkDensity_;
    result.lcsScore = lcsScore_;
    result.saScore = saScore_;
    result.finalScore = finalScore();
    result.verdict = verdict(result.finalScore);
    return result;
}
