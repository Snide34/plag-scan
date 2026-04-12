#include "preprocessor.h"
#include "pattern_matcher.h"
#include "similarity_engine.h"
#include "scorer.h"
#include "file_io.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <map>

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " <file1> <file2> [options]\n"
              << "\nOptions:\n"
              << "  --output <path>    Save full report to file (JSON format)\n"
              << "  --mode <type>      Force processing mode: text|code|auto (default: auto)\n"
              << "  --help             Show this help message\n"
              << "\nExample:\n"
              << "  " << programName << " doc1.txt doc2.txt --output report.json\n"
              << "  " << programName << " code1.cpp code2.cpp --mode code\n";
}

void printSummaryTable(const ScorerResult& result) {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════╗\n";
    std::cout << "║       PLAGIARISM DETECTION SUMMARY             ║\n";
    std::cout << "╚════════════════════════════════════════════════╝\n\n";
    
    std::cout << std::left << std::setw(20) << "Algorithm" << " | " << "Score\n";
    std::cout << std::string(50, '-') << "\n";
    
    std::cout << std::left << std::setw(20) << "KMP density" << " | " 
              << std::fixed << std::setprecision(2) << result.kmpDensity << "\n";
    std::cout << std::left << std::setw(20) << "Rabin-Karp" << " | " 
              << std::fixed << std::setprecision(2) << result.rkDensity << "\n";
    std::cout << std::left << std::setw(20) << "LCS" << " | " 
              << std::fixed << std::setprecision(2) << result.lcsScore << "\n";
    std::cout << std::left << std::setw(20) << "Suffix Array" << " | " 
              << std::fixed << std::setprecision(2) << result.saScore << "\n";
    
    std::cout << std::string(50, '=') << "\n";
    std::cout << std::left << std::setw(20) << "FINAL" << " | " 
              << std::fixed << std::setprecision(2) << result.finalScore 
              << "  →  " << result.verdict << "\n";
    std::cout << "\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string file1Path = argv[1];
    std::string file2Path = argv[2];
    
    // Parse options
    std::string outputPath;
    std::string mode = "auto";
    
    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "--output" && i + 1 < argc) {
            outputPath = argv[++i];
        } else if (arg == "--mode" && i + 1 < argc) {
            mode = argv[++i];
            if (mode != "text" && mode != "code" && mode != "auto") {
                std::cerr << "Error: Invalid mode '" << mode << "'. Use text, code, or auto.\n";
                return 1;
            }
        }
    }
    
    // Error handling
    FileIO fileIO;
    
    if (!fileIO.fileExists(file1Path)) {
        std::cerr << "Error: File not found: " << file1Path << "\n";
        return 1;
    }
    
    if (!fileIO.fileExists(file2Path)) {
        std::cerr << "Error: File not found: " << file2Path << "\n";
        return 1;
    }
    
    if (file1Path == file2Path) {
        std::cerr << "Error: Same file passed twice. Please provide two different files.\n";
        return 1;
    }
    
    try {
        // Step 1: Read files
        std::string content1 = fileIO.readFile(file1Path);
        std::string content2 = fileIO.readFile(file2Path);
        
        if (content1.empty()) {
            std::cerr << "Error: File is empty: " << file1Path << "\n";
            return 1;
        }
        
        if (content2.empty()) {
            std::cerr << "Error: File is empty: " << file2Path << "\n";
            return 1;
        }
        
        // Step 2: Detect or set file type
        bool isCode;
        if (mode == "auto") {
            FileType type1 = fileIO.detectType(file1Path);
            FileType type2 = fileIO.detectType(file2Path);
            isCode = (type1 == FileType::CODE || type2 == FileType::CODE);
        } else {
            isCode = (mode == "code");
        }
        
        // Step 3: Preprocess
        std::string processed1, processed2;
        if (isCode) {
            CodePreprocessor proc;
            processed1 = proc.preprocess(content1, true);
            processed2 = proc.preprocess(content2, true);
        } else {
            TextPreprocessor proc;
            processed1 = proc.preprocess(content1, false);
            processed2 = proc.preprocess(content2, false);
        }
        
        // Step 4: Run pattern matching algorithms
        KMP kmp;
        RabinKarp rk;
        
        // Use processed2 as pattern to search in processed1
        std::vector<size_t> kmpMatches = kmp.search(processed1, processed2);
        std::vector<size_t> rkMatches = rk.search(processed1, processed2);
        
        double kmpDensity = kmp.matchDensity(processed1, processed2);
        double rkDensity = rk.matchDensity(processed1, processed2);
        
        // Step 5: Run similarity algorithms
        LCS lcs;
        SuffixArray sa;
        SimilarityEngine simEngine;
        
        double lcsScore = lcs.similarityScore(processed1, processed2);
        
        std::string longestCommon = sa.longestCommonSubstring(processed1, processed2);
        double saScore = 0.0;
        if (!longestCommon.empty() && !processed1.empty()) {
            saScore = static_cast<double>(longestCommon.length()) / 
                     static_cast<double>(std::max(processed1.length(), processed2.length()));
        }
        
        auto matchSpans = simEngine.highlightMatches(content1, content2);
        
        // Step 6: Score and generate report
        Scorer scorer(kmpDensity, rkDensity, lcsScore, saScore);
        ScorerResult result = scorer.getResult();
        
        ReportGenerator reportGen;
        
        // Step 7: Print summary to stdout
        printSummaryTable(result);
        
        // Save full report if requested
        if (!outputPath.empty()) {
            std::string jsonReport = reportGen.toJSON(result, matchSpans, content1, content2);
            
            if (fileIO.writeFile(outputPath, jsonReport)) {
                std::cout << "Full report saved to: " << outputPath << "\n";
            } else {
                std::cerr << "Warning: Failed to save report to " << outputPath << "\n";
            }
        }
        
        // Return exit code based on verdict
        if (result.finalScore >= 0.70) {
            return 2; // High plagiarism
        } else if (result.finalScore >= 0.45) {
            return 1; // Moderate plagiarism
        }
        
        return 0; // Original or low similarity
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
