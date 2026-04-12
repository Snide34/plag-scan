# Plagiarism Detector - Demo Guide for Presentation

## Quick Setup (5 minutes)

### Prerequisites
- C++ compiler (g++, clang, or MSVC)
- Terminal/Command Prompt

### Build Instructions

**Option 1: Using g++ (Recommended for quick demo)**
```bash
g++ -std=c++17 -I include src/*.cpp -o plagiarism_detector
```

**Option 2: Using CMake (if available)**
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

---

## Demo Script (10-15 minutes)

### 1. Introduction (2 minutes)
"Today I'll demonstrate a multi-algorithm plagiarism detection system that analyzes both text and code documents using four different algorithms."

### 2. Show Project Structure (1 minute)
```bash
tree /F
# Or manually show:
# - include/ (headers)
# - src/ (implementations)
# - data/ (test samples)
# - tests/ (unit tests)
# - docs/ (algorithm analysis)
```

### 3. Demo 1: Identical Documents (1 minute)
**Command:**
```bash
./plagiarism_detector data/sample1.txt data/sample1.txt
```

**Expected Output:**
```
Algorithm            | Score
--------------------------------------------------
KMP density          | 1.00
Rabin-Karp           | 1.00
LCS                  | 1.00
Suffix Array         | 1.00
==================================================
FINAL                | 1.00  →  High plagiarism
```

**Explain:** "Perfect match - all algorithms detect 100% similarity."

---

### 4. Demo 2: Paraphrased Text (2 minutes)
**Command:**
```bash
./plagiarism_detector data/sample1.txt data/sample2.txt
```

**Expected Output:**
```
Algorithm            | Score
--------------------------------------------------
KMP density          | 0.00
Rabin-Karp           | 0.00
LCS                  | 0.52
Suffix Array         | 0.03
==================================================
FINAL                | 0.19  →  Original
```

**Explain:** 
- "These are paraphrased documents about AI/ML"
- "Pattern matching (KMP, RK) finds no exact matches"
- "LCS detects some structural similarity (0.52)"
- "Final weighted score: 0.19 - classified as Original"
- "This shows the system doesn't flag legitimate paraphrasing"

---

### 5. Demo 3: Code Plagiarism with Renamed Variables (3 minutes)
**Command:**
```bash
./plagiarism_detector data/sample2_original.cpp data/sample2_plagiarised.cpp --mode code
```

**Expected Output:**
```
Algorithm            | Score
--------------------------------------------------
KMP density          | 1.00
Rabin-Karp           | 1.00
LCS                  | 1.00
Suffix Array         | 1.00
==================================================
FINAL                | 1.00  →  High plagiarism
```

**Show the files side by side:**
```bash
# Show original
cat data/sample2_original.cpp

# Show plagiarised (renamed variables)
cat data/sample2_plagiarised.cpp
```

**Explain:**
- "Original has variables: arr, n, i, j, temp, swapped"
- "Plagiarised has: myArray, arraySize, outerIndex, innerIndex, temporary, hasSwapped"
- "Comments reordered, but algorithm is identical"
- "Our preprocessor normalizes identifiers to VAR_0, VAR_1, etc."
- "Result: Perfect detection despite variable renaming!"

---

### 6. Demo 4: Similar but Different Code (2 minutes)
**Command:**
```bash
./plagiarism_detector data/sample_code1.cpp data/sample_code2.cpp
```

**Expected Output:**
```
Algorithm            | Score
--------------------------------------------------
KMP density          | 0.00
Rabin-Karp           | 0.00
LCS                  | 0.99
Suffix Array         | 0.89
==================================================
FINAL                | 0.57  →  Moderate plagiarism
```

**Explain:**
- "Both implement factorial and findMax functions"
- "Similar structure but legitimate independent work"
- "Moderate similarity (0.57) - would warrant manual review"

---

### 7. Demo 5: Generate JSON Report (2 minutes)
**Command:**
```bash
./plagiarism_detector data/sample_code1.cpp data/sample_code2.cpp --output demo_report.json
```

**Show the report:**
```bash
cat demo_report.json
# Or open in text editor
```

**Explain:**
- "Machine-readable JSON format"
- "Contains algorithm breakdown"
- "Lists matched regions with excerpts"
- "Can be integrated into larger systems"

---

### 8. Demo 6: Completely Different Documents (1 minute)
**Command:**
```bash
./plagiarism_detector data/sample1.txt data/sample2_original.cpp
```

**Expected Output:**
```
FINAL                | 0.19  →  Original
```

**Explain:** "Text vs code - correctly identified as unrelated."

---

### 9. Show Algorithm Documentation (2 minutes)
**Open:** `docs/analysis.md`

**Highlight:**
- Complexity table (KMP: O(n+m), LCS: O(nm), etc.)
- Algorithm selection justification
- Trade-off analysis
- Future improvements

**Say:** "I've documented the design decisions, complexity analysis, and trade-offs between algorithms."

---

### 10. Show Test Coverage (Optional - 1 minute)
**If you have GoogleTest installed:**
```bash
cd build
cmake -DBUILD_TESTS=ON ..
cmake --build .
ctest
```

**Expected:** All tests pass (60+ unit tests + integration tests)

---

## Key Points to Emphasize

### 1. Multi-Algorithm Approach
- **KMP (20%)**: Exact pattern matching, O(n+m) guaranteed
- **Rabin-Karp (20%)**: Rolling hash, handles multiple patterns
- **LCS (35%)**: Detects paraphrasing and reordering
- **Suffix Array (25%)**: Finds all common substrings efficiently

### 2. Preprocessing Intelligence
- **Text**: Tokenization, stopword removal, Porter stemming
- **Code**: Comment stripping, identifier normalization
- Makes detection robust against superficial changes

### 3. Weighted Scoring System
- LCS gets highest weight (35%) for paraphrase detection
- Pattern matching (40% combined) for exact copying
- Suffix array (25%) for substring analysis
- Produces interpretable verdicts

### 4. Practical Features
- Auto file type detection
- JSON report generation
- Clean CLI interface
- Exit codes for automation

---

## Backup Demos (If Time Permits)

### Show Help Message
```bash
./plagiarism_detector --help
```

### Error Handling
```bash
# File not found
./plagiarism_detector nonexistent.txt data/sample1.txt

# Same file twice
./plagiarism_detector data/sample1.txt data/sample1.txt
```

---

## Questions Your Teacher Might Ask

### Q1: "Why use multiple algorithms?"
**A:** "Each algorithm has strengths:
- KMP/RK catch exact copying
- LCS detects paraphrasing
- Suffix Array finds all common substrings
- Combined, they provide comprehensive detection"

### Q2: "What's the time complexity?"
**A:** "Dominated by LCS at O(nm), but optimized to O(min(n,m)) space. For typical documents (1K-50K words), it runs in under a second. Suffix array is O(n log n) which scales better for very large documents."

### Q3: "How do you handle false positives?"
**A:** "Preprocessing removes common words and normalizes text. The weighted scoring prevents any single algorithm from dominating. We have four verdict levels (Original, Low, Moderate, High) to indicate confidence."

### Q4: "Can it detect cross-language plagiarism?"
**A:** "Currently no - it's a limitation I documented. Future improvement would be converting code to AST or control flow graphs for language-independent comparison."

### Q5: "How accurate is it?"
**A:** "Integration tests show:
- 100% detection on identical code
- 100% detection on renamed variables
- Correctly identifies paraphrased content as original
- Moderate scores for legitimately similar work"

---

## Presentation Tips

1. **Practice the demo beforehand** - make sure all commands work
2. **Have the terminal ready** with large font size
3. **Keep sample files open** in an editor to show side-by-side
4. **Prepare the JSON report** beforehand if time is tight
5. **Have docs/analysis.md open** in a browser/editor
6. **Print this guide** as a reference during demo
7. **Time yourself** - aim for 10-12 minutes, leaving time for questions

---

## Troubleshooting

### If compilation fails:
```bash
# Try compiling one file at a time to find the error
g++ -std=c++17 -I include -c src/preprocessor.cpp
g++ -std=c++17 -I include -c src/kmp.cpp
# etc.
```

### If program crashes:
- Check file paths are correct
- Ensure data/ directory exists
- Verify sample files are present

### If output looks wrong:
- Check terminal encoding (UTF-8)
- Try redirecting to file: `./plagiarism_detector ... > output.txt`

---

## Post-Demo Discussion Points

1. **Real-world applications**: Academic integrity, code review, copyright protection
2. **Scalability**: Current system handles documents up to ~50K words efficiently
3. **Extensions**: Could add web interface, database integration, batch processing
4. **Machine learning**: Future version could use BERT embeddings for semantic similarity

---

## Closing Statement

"This project demonstrates practical application of multiple algorithms from our DAA course:
- Dynamic programming (LCS)
- String matching (KMP, Rabin-Karp)
- Advanced data structures (Suffix Arrays)
- Algorithm analysis and trade-offs

The system is production-ready with comprehensive testing, documentation, and a clean CLI interface. Thank you!"

---

## Quick Reference Commands

```bash
# Build
g++ -std=c++17 -I include src/*.cpp -o plagiarism_detector

# Basic usage
./plagiarism_detector file1.txt file2.txt

# With report
./plagiarism_detector file1.cpp file2.cpp --output report.json

# Force mode
./plagiarism_detector file1 file2 --mode code

# Help
./plagiarism_detector --help
```

---

**Good luck with your demo! 🚀**
