# Quick Start Guide - 5 Minutes to Demo

## Step 1: Build (30 seconds)
```bash
g++ -std=c++17 -I include src/*.cpp -o plagiarism_detector
```

## Step 2: Test Run (30 seconds)
```bash
# Test 1: Identical files
./plagiarism_detector data/sample1.txt data/sample1.txt
# Expected: 1.00 → High plagiarism

# Test 2: Different files
./plagiarism_detector data/sample1.txt data/sample2_original.cpp
# Expected: 0.19 → Original

# Test 3: Plagiarized code
./plagiarism_detector data/sample2_original.cpp data/sample2_plagiarised.cpp --mode code
# Expected: 1.00 → High plagiarism
```

## Step 3: Generate Report (30 seconds)
```bash
./plagiarism_detector data/sample_code1.cpp data/sample_code2.cpp --output report.json
cat report.json
```

## Demo Commands (Copy-Paste Ready)

### Command 1: Show Help
```bash
./plagiarism_detector --help
```

### Command 2: Paraphrased Text
```bash
./plagiarism_detector data/sample1.txt data/sample2.txt
```

### Command 3: Code Plagiarism
```bash
./plagiarism_detector data/sample2_original.cpp data/sample2_plagiarised.cpp --mode code
```

### Command 4: With JSON Report
```bash
./plagiarism_detector data/sample_code1.cpp data/sample_code2.cpp --output demo_report.json
```

### Command 5: Show Report
```bash
cat demo_report.json
```

## What to Say During Demo

**Opening:**
"I've built a plagiarism detection system using 4 algorithms: KMP, Rabin-Karp, LCS, and Suffix Arrays."

**Demo 1 (Identical):**
"First, let's test identical documents. As expected, all algorithms score 1.00 - perfect match."

**Demo 2 (Paraphrased):**
"Now paraphrased text about AI. Pattern matching finds nothing, but LCS detects 52% structural similarity. Final score: 0.19 - correctly classified as Original."

**Demo 3 (Code):**
"Here's the key demo - same bubble sort algorithm but with renamed variables. Our preprocessor normalizes identifiers, so we get 100% detection despite the changes."

**Demo 4 (Report):**
"The system generates machine-readable JSON reports with algorithm breakdown and matched regions."

**Closing:**
"The system combines multiple algorithms with weighted scoring to provide robust plagiarism detection for both text and code."

## Troubleshooting

**If build fails:**
```bash
# Check compiler
g++ --version

# Try manual compilation
g++ -std=c++17 -I include -c src/preprocessor.cpp -o preprocessor.o
g++ -std=c++17 -I include -c src/kmp.cpp -o kmp.o
# ... continue for all files
g++ *.o -o plagiarism_detector
```

**If files not found:**
```bash
# Check you're in project root
ls data/
# Should show: sample1.txt, sample2.txt, sample2_original.cpp, etc.
```

**If output looks garbled:**
```bash
# Redirect to file
./plagiarism_detector data/sample1.txt data/sample2.txt > output.txt
cat output.txt
```

## Time Allocation (10 min demo)

- 0:00-1:00 - Introduction & build
- 1:00-2:00 - Demo 1: Identical documents
- 2:00-4:00 - Demo 2: Paraphrased text (explain algorithms)
- 4:00-7:00 - Demo 3: Code plagiarism (show files, explain preprocessing)
- 7:00-8:00 - Demo 4: JSON report
- 8:00-9:00 - Show docs/analysis.md
- 9:00-10:00 - Wrap up & questions

## Emergency Backup

If live demo fails, show pre-captured output:

**Identical Documents:**
```
FINAL                | 1.00  →  High plagiarism
```

**Paraphrased Text:**
```
KMP density          | 0.00
Rabin-Karp           | 0.00
LCS                  | 0.52
Suffix Array         | 0.03
FINAL                | 0.19  →  Original
```

**Code Plagiarism:**
```
FINAL                | 1.00  →  High plagiarism
```

Good luck! 🚀
