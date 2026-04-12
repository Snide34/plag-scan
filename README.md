# Pattern & Plagiarism Detector

A C++17 project for detecting patterns and plagiarism in text and code documents.

## Overview

This project provides preprocessing capabilities for both natural language text and source code, enabling similarity detection and plagiarism analysis. The system normalizes inputs to facilitate accurate comparison and pattern matching.

## Features

- **Text Preprocessing**: Tokenization, stopword removal, stemming, and normalization
- **Code Preprocessing**: Comment stripping, code tokenization, and identifier normalization
- **Pattern Matching**: KMP and Rabin-Karp algorithms for efficient string searching
- **Multiple Pattern Search**: Simultaneous search for multiple patterns using Rabin-Karp
- **Match Density Analysis**: Calculate pattern coverage in text
- **Modular Design**: Separate classes for text, code, and pattern matching
- **C++17 Standard**: Modern C++ features and best practices
- **Optional Testing**: GoogleTest integration for unit testing

## Documentation

See [docs/analysis.md](docs/analysis.md) for detailed algorithm analysis, complexity tables, and design decisions.

## Project Structure

```
.
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── include/                # Header files
│   └── preprocessor.h      # Preprocessor class declarations
├── src/                    # Source files
│   ├── preprocessor.cpp    # Preprocessor implementations
│   └── main.cpp            # Main application entry point
├── tests/                  # Test files
│   └── preprocessor_test.cpp
└── data/                   # Sample data files
```

## Building

### Prerequisites

- CMake 3.14 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- GoogleTest (optional, for testing)

### Build Instructions

```bash
# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build
cmake --build .

# Run the application
./detector_app
```

### Building with Tests

```bash
mkdir build && cd build
cmake -DBUILD_TESTS=ON ..
cmake --build .
ctest
```

## Usage

### Command Line Interface

```bash
# Basic usage
./plagiarism_detector file1.txt file2.txt

# Save full report to JSON
./plagiarism_detector doc1.txt doc2.txt --output report.json

# Force processing mode
./plagiarism_detector code1.cpp code2.cpp --mode code

# Auto-detect file type (default)
./plagiarism_detector file1.txt file2.cpp --mode auto
```

### Sample Output

```
╔════════════════════════════════════════════════╗
║       PLAGIARISM DETECTION SUMMARY             ║
╚════════════════════════════════════════════════╝

Algorithm            | Score
--------------------------------------------------
KMP density          | 0.34
Rabin-Karp           | 0.31
LCS                  | 0.58
Suffix Array         | 0.61
==================================================
FINAL                | 0.48  →  Moderate plagiarism
```

### Exit Codes

- `0`: Original or low similarity
- `1`: Moderate plagiarism or error
- `2`: High plagiarism

### Programmatic Usage

```cpp
#include "preprocessor.h"
#include "pattern_matcher.h"
#include "similarity_engine.h"
#include "scorer.h"

// Preprocess documents
TextPreprocessor proc;
std::string doc1 = proc.preprocess(text1, false);
std::string doc2 = proc.preprocess(text2, false);

// Run algorithms
KMP kmp;
double kmpDensity = kmp.matchDensity(doc1, doc2);

LCS lcs;
double lcsScore = lcs.similarityScore(doc1, doc2);

// Score and report
Scorer scorer(kmpDensity, rkDensity, lcsScore, saScore);
ScorerResult result = scorer.getResult();
```

## Components

### TextPreprocessor

- `tokenize()`: Splits text into individual tokens
- `removeStopwords()`: Filters common words (the, is, a, etc.)
- `stemWord()`: Reduces words to their root form (basic Porter stemmer)
- `normalize()`: Converts to lowercase and removes punctuation
- `preprocess()`: Complete preprocessing pipeline

### CodePreprocessor

- `stripComments()`: Removes single-line (//) and multi-line (/* */) comments
- `tokenizeCode()`: Splits code while preserving keywords and operators
- `normalizeIdentifiers()`: Replaces variable names with generic placeholders (VAR_0, VAR_1, etc.)
- `preprocess()`: Complete code preprocessing pipeline

### KMP (Knuth-Morris-Pratt)

- `buildFailureFunction()`: Constructs the LPS (Longest Proper Prefix which is also Suffix) array
- `search()`: Finds all occurrences of pattern in text
- **Time Complexity**: O(n + m) where n = text length, m = pattern length
- **Space Complexity**: O(m) for failure function

### RabinKarp

- `search()`: Single pattern search using rolling hash
- `searchMultiple()`: Efficiently search for multiple patterns simultaneously
- **Time Complexity**: O(n + m) average case, O(nm) worst case
- **Space Complexity**: O(1) for single pattern, O(k) for k patterns
- **Hash Collision Handling**: Includes verification step to ensure correctness

### PatternMatcher Interface

- `search()`: Virtual method for pattern searching
- `matchDensity()`: Calculates pattern coverage ratio: (matches × patternLen) / textLen

## Components

### TextPreprocessor

- `tokenize()`: Splits text into individual tokens
- `removeStopwords()`: Filters common words (the, is, a, etc.)
- `stemWord()`: Reduces words to their root form (basic Porter stemmer)
- `normalize()`: Converts to lowercase and removes punctuation
- `preprocess()`: Complete preprocessing pipeline

### CodePreprocessor

- `stripComments()`: Removes single-line (//) and multi-line (/* */) comments
- `tokenizeCode()`: Splits code while preserving keywords and operators
- `normalizeIdentifiers()`: Replaces variable names with generic placeholders (VAR_0, VAR_1, etc.)
- `preprocess()`: Complete code preprocessing pipeline

## License

MIT License

## Contributing

Contributions are welcome! Please submit pull requests or open issues for bugs and feature requests.
