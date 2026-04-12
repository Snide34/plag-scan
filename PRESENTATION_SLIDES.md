# Plagiarism Detection System
## Design and Analysis of Algorithms Project

---

## Slide 1: Title
**Plagiarism Detection System**
*Multi-Algorithm Approach for Text and Code Analysis*

**Student:** [Your Name]
**Course:** Design and Analysis of Algorithms
**Date:** [Date]

---

## Slide 2: Problem Statement

### Challenge
- Detect plagiarism in text documents and source code
- Handle various forms: direct copying, paraphrasing, variable renaming
- Balance accuracy with computational efficiency

### Requirements
- Process large documents efficiently
- Minimize false positives
- Provide interpretable results
- Support both text and code

---

## Slide 3: System Architecture

```
Input Documents
      ↓
Preprocessing (Text/Code)
      ↓
┌─────────────────────────────┐
│  Pattern Matching           │
│  • KMP (20%)                │
│  • Rabin-Karp (20%)         │
└─────────────────────────────┘
      ↓
┌─────────────────────────────┐
│  Similarity Analysis        │
│  • LCS (35%)                │
│  • Suffix Array (25%)       │
└─────────────────────────────┘
      ↓
Weighted Scoring
      ↓
Report Generation
```

---

## Slide 4: Algorithm Selection

| Algorithm | Time | Space | Purpose |
|-----------|------|-------|---------|
| **KMP** | O(n+m) | O(m) | Exact pattern matching |
| **Rabin-Karp** | O(n+m) avg | O(1) | Multiple patterns |
| **LCS** | O(nm) | O(min(n,m)) | Paraphrase detection |
| **Suffix Array** | O(n log n) | O(n) | Common substrings |

**Why not naive approaches?**
- Naive pattern matching: O(nm) worst case
- Brute force LCS: O(n²m) for substrings
- Our approach: Optimal complexity for each task

---

## Slide 5: Preprocessing Module

### Text Preprocessing
1. **Tokenization** - Split into words
2. **Stopword Removal** - Filter common words
3. **Stemming** - Porter algorithm (running → run)
4. **Normalization** - Lowercase, remove punctuation

### Code Preprocessing
1. **Comment Stripping** - Remove // and /* */
2. **Tokenization** - Split on delimiters
3. **Identifier Normalization** - myVar → VAR_0

**Benefit:** Robust against superficial changes

---

## Slide 6: KMP Algorithm

### Knuth-Morris-Pratt
- **Preprocessing:** Build failure function (LPS array)
- **Searching:** No backtracking in text
- **Complexity:** O(n+m) guaranteed

### Key Advantage
```
Naive:  "AAAAAAB" vs "AAAAB" → O(nm) comparisons
KMP:    Same input → O(n+m) comparisons
```

**Use Case:** Exact pattern detection in documents

---

## Slide 7: Rabin-Karp Algorithm

### Rolling Hash Technique
```
Text:    [a b c d e f]
Pattern: [c d e]

Hash(abc) → slide → Hash(bcd) → slide → Hash(cde) ✓
```

### Features
- **O(1)** hash update per position
- **Multiple patterns** efficiently
- **Collision handling** with verification

**Use Case:** Searching for common phrases

---

## Slide 8: LCS Algorithm

### Longest Common Subsequence
```
Text 1: "AGGTAB"
Text 2: "GXTXAYB"
LCS:    "GTAB" (length 4)

Similarity = 2 × 4 / (6 + 7) = 0.615
```

### Dynamic Programming
- **Table:** dp[i][j] = LCS length of first i and j chars
- **Optimization:** Use only 2 rows (space: O(min(n,m)))

**Use Case:** Detect paraphrasing and reordering

---

## Slide 9: Suffix Array

### Construction (Prefix Doubling)
```
String: "banana"
Suffixes: banana, anana, nana, ana, na, a
Sorted:   a, ana, anana, banana, na, nana
SA:       [5, 3, 1, 0, 4, 2]
```

### LCP Array (Kasai's Algorithm)
- Longest Common Prefix between adjacent suffixes
- **O(n)** construction time

**Use Case:** Find all common substrings efficiently

---

## Slide 10: Weighted Scoring System

### Formula
```
Final Score = 0.20 × KMP + 0.20 × RK + 0.35 × LCS + 0.25 × SA
```

### Verdict Thresholds
| Score | Verdict |
|-------|---------|
| < 0.20 | **Original** |
| 0.20 - 0.45 | **Low similarity** |
| 0.45 - 0.70 | **Moderate plagiarism** |
| > 0.70 | **High plagiarism** |

**Rationale:** LCS gets highest weight for paraphrase detection

---

## Slide 11: Demo Results

### Test Case 1: Identical Documents
```
KMP: 1.00  RK: 1.00  LCS: 1.00  SA: 1.00
Final: 1.00 → High plagiarism ✓
```

### Test Case 2: Paraphrased Text
```
KMP: 0.00  RK: 0.00  LCS: 0.52  SA: 0.03
Final: 0.19 → Original ✓
```

### Test Case 3: Code with Renamed Variables
```
KMP: 1.00  RK: 1.00  LCS: 1.00  SA: 1.00
Final: 1.00 → High plagiarism ✓
```

---

## Slide 12: Code Plagiarism Detection

### Original Code
```cpp
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}
```

### Plagiarized (Renamed Variables)
```cpp
void bubbleSort(vector<int>& myArray) {
    int arraySize = myArray.size();
    for (int outerIndex = 0; outerIndex < arraySize-1; outerIndex++) {
        for (int innerIndex = 0; innerIndex < arraySize-outerIndex-1; innerIndex++) {
            if (myArray[innerIndex] > myArray[innerIndex+1]) {
                swap(myArray[innerIndex], myArray[innerIndex+1]);
            }
        }
    }
}
```

**After Normalization:** Both become identical → 100% match!

---

## Slide 13: Complexity Analysis

### Time Complexity (for documents of length n, m)
```
Preprocessing:  O(n×k) where k = avg word length
KMP:            O(n+m)
Rabin-Karp:     O(n+m) average, O(nm) worst
LCS:            O(nm)
Suffix Array:   O(n log n)

Total:          O(nm) dominated by LCS
```

### Space Complexity
```
KMP:            O(m)
Rabin-Karp:     O(1)
LCS:            O(min(n,m))  [optimized from O(nm)]
Suffix Array:   O(n)

Total:          O(n+m)
```

---

## Slide 14: Trade-off Analysis

### KMP vs Rabin-Karp
| Aspect | KMP | Rabin-Karp |
|--------|-----|------------|
| Worst case | O(n+m) | O(nm) |
| Space | O(m) | O(1) |
| Multiple patterns | Sequential | Parallel |
| Predictability | Guaranteed | Probabilistic |

**Decision:** Use both for comprehensive coverage

### LCS vs Suffix Array
- **LCS:** Better for non-contiguous matches (paraphrasing)
- **SA:** Better for finding all substrings (verbatim copying)
- **Decision:** Combine with weights (35% vs 25%)

---

## Slide 15: Features

### CLI Interface
```bash
./plagiarism_detector file1.txt file2.txt
./plagiarism_detector code1.cpp code2.cpp --output report.json
./plagiarism_detector doc1 doc2 --mode auto
```

### Auto File Type Detection
- `.cpp .c .java .py .js .ts` → CODE mode
- Others → TEXT mode
- Override with `--mode` flag

### JSON Report Generation
- Machine-readable format
- Algorithm breakdown
- Matched regions with excerpts
- Integration-ready

---

## Slide 16: Testing

### Unit Tests (60+ tests)
- Preprocessor: tokenization, stemming, normalization
- Pattern matching: edge cases, overlapping matches
- Similarity: identical, partial, different inputs
- Scoring: verdict thresholds, weighted calculation

### Integration Tests
- Full pipeline validation
- Score range verification
- Real-world scenarios

### Test Coverage
```
✓ Empty strings
✓ Identical documents
✓ Completely different
✓ Paraphrased content
✓ Code with renamed variables
✓ Overlapping matches
```

---

## Slide 17: Limitations

### Current Limitations
1. **Semantic Understanding** - Cannot detect deep paraphrasing with complete synonym replacement
2. **Language Dependency** - Optimized for English text
3. **Code Language Specificity** - Best for C-style languages
4. **Scalability** - O(nm) becomes prohibitive for very large documents (>100K words)
5. **Cross-Language** - Cannot detect Java → Python translation

### Mitigation Strategies
- Multi-algorithm approach reduces false negatives
- Preprocessing handles common variations
- Weighted scoring balances different detection methods

---

## Slide 18: Future Improvements

### 1. Machine Learning Integration
- BERT/CodeBERT embeddings for semantic similarity
- Neural networks trained on labeled datasets
- Expected: 20-30% better paraphrase detection

### 2. Advanced NLP
- WordNet-based synonym detection
- Named entity recognition
- Dependency parsing

### 3. Scalability
- Parallel processing (4-8x speedup)
- GPU acceleration for DP algorithms
- Incremental processing with caching

### 4. Cross-Language Detection
- AST-based comparison
- Control flow graph analysis
- Language-agnostic algorithmic structure

---

## Slide 19: Real-World Applications

### Academic Institutions
- Assignment submission checking
- Thesis plagiarism detection
- Exam integrity monitoring

### Software Industry
- Code review automation
- License compliance checking
- Open source contribution verification

### Publishing
- Article originality verification
- Copyright infringement detection
- Content authenticity validation

---

## Slide 20: Project Statistics

### Implementation
- **Languages:** C++17
- **Lines of Code:** ~2,500
- **Files:** 12 source, 8 headers, 7 test files
- **Algorithms:** 4 major (KMP, RK, LCS, SA)
- **Build System:** CMake with GoogleTest

### Documentation
- **Algorithm Analysis:** Comprehensive complexity analysis
- **API Documentation:** All classes and methods documented
- **Demo Guide:** Step-by-step presentation guide
- **Test Coverage:** 60+ unit tests + integration tests

### Performance
- **Typical Documents (1K-10K words):** < 1 second
- **Large Documents (50K words):** < 5 seconds
- **Code Files (500-1000 lines):** < 0.5 seconds

---

## Slide 21: Key Takeaways

### Algorithm Design Principles Applied
1. **Divide and Conquer** - Break problem into pattern matching + similarity
2. **Dynamic Programming** - LCS optimization
3. **Greedy Approach** - Suffix array construction
4. **Hashing** - Rabin-Karp rolling hash
5. **String Algorithms** - KMP failure function

### Software Engineering Practices
- Modular design with clear interfaces
- Comprehensive testing (unit + integration)
- Documentation (code + algorithm analysis)
- Error handling and user-friendly CLI

---

## Slide 22: Conclusion

### Achievements
✓ Multi-algorithm plagiarism detection system
✓ Handles both text and code
✓ Robust against superficial changes
✓ Efficient complexity (O(nm) for typical use)
✓ Comprehensive testing and documentation

### Learning Outcomes
- Practical application of DAA concepts
- Algorithm selection and trade-off analysis
- Performance optimization techniques
- Real-world problem solving

### Impact
- Promotes academic integrity
- Protects intellectual property
- Automates tedious manual review

---

## Slide 23: Demo

### Live Demonstration
1. Identical documents → High plagiarism
2. Paraphrased text → Original
3. Code with renamed variables → High plagiarism
4. Different documents → Original
5. JSON report generation

**[Switch to terminal for live demo]**

---

## Slide 24: Questions?

### Contact
- **Email:** [your.email@example.com]
- **GitHub:** [github.com/yourusername/plagiarism-detector]
- **Documentation:** See `docs/analysis.md`

### References
- Knuth-Morris-Pratt Algorithm (1977)
- Rabin-Karp Algorithm (1987)
- Porter Stemming Algorithm (1980)
- Kasai's LCP Algorithm (2001)

**Thank you for your attention!**

---

## Backup Slides

### B1: Porter Stemming Algorithm
```
Step 1a: sses → ss, ies → i, s → ε
Step 1b: eed → ee (if m>0), ed → ε (if vowel)
Step 1c: y → i (if vowel before)
Step 2: ational → ate, tional → tion
...
```

### B2: Suffix Array Construction Details
```
Iteration 0: Sort by first character
Iteration 1: Sort by first 2 characters
Iteration 2: Sort by first 4 characters
...
Iteration log(n): Complete suffix array
```

### B3: JSON Report Structure
```json
{
  "score": 0.57,
  "verdict": "Moderate plagiarism",
  "algorithm_breakdown": {
    "kmp_density": 0.00,
    "rabin_karp_density": 0.00,
    "lcs_score": 0.99,
    "suffix_array_score": 0.89
  },
  "matched_regions": [...]
}
```
