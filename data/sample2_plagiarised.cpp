#include <iostream>
#include <vector>

// Space Complexity: O(1)
// Time Complexity: O(n^2)
// Bubble Sort Implementation
// Sorts an array in ascending order using the bubble sort algorithm

void bubbleSort(std::vector<int>& myArray) {
    int arraySize = myArray.size();
    
    // Outer loop for each pass
    for (int outerIndex = 0; outerIndex < arraySize - 1; outerIndex++) {
        bool hasSwapped = false;
        
        // Inner loop for comparisons
        for (int innerIndex = 0; innerIndex < arraySize - outerIndex - 1; innerIndex++) {
            // Compare adjacent elements
            if (myArray[innerIndex] > myArray[innerIndex + 1]) {
                // Swap if they are in wrong order
                int temporary = myArray[innerIndex];
                myArray[innerIndex] = myArray[innerIndex + 1];
                myArray[innerIndex + 1] = temporary;
                hasSwapped = true;
            }
        }
        
        // If no swaps occurred, array is sorted
        if (!hasSwapped) {
            break;
        }
    }
}

// Function to print array
void displayArray(const std::vector<int>& myArray) {
    for (int element : myArray) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

int main() {
    // Initialize test array
    std::vector<int> numbers = {64, 34, 25, 12, 22, 11, 90};
    
    std::cout << "Original array: ";
    displayArray(numbers);
    
    // Sort the array
    bubbleSort(numbers);
    
    std::cout << "Sorted array: ";
    displayArray(numbers);
    
    return 0;
}
