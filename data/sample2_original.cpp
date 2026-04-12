#include <iostream>
#include <vector>

/**
 * Bubble Sort Implementation
 * Sorts an array in ascending order using the bubble sort algorithm
 * Time Complexity: O(n^2)
 * Space Complexity: O(1)
 */

// Function to perform bubble sort
void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    
    // Outer loop for each pass
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        
        // Inner loop for comparisons
        for (int j = 0; j < n - i - 1; j++) {
            // Compare adjacent elements
            if (arr[j] > arr[j + 1]) {
                // Swap if they are in wrong order
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        
        // If no swaps occurred, array is sorted
        if (!swapped) {
            break;
        }
    }
}

// Function to print array
void printArray(const std::vector<int>& arr) {
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

int main() {
    // Initialize test array
    std::vector<int> data = {64, 34, 25, 12, 22, 11, 90};
    
    std::cout << "Original array: ";
    printArray(data);
    
    // Sort the array
    bubbleSort(data);
    
    std::cout << "Sorted array: ";
    printArray(data);
    
    return 0;
}
