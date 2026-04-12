#include <iostream>
#include <vector>

// Function to calculate factorial
int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

// Function to find maximum element
int findMax(const std::vector<int>& arr) {
    int maxVal = arr[0];
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] > maxVal) {
            maxVal = arr[i];
        }
    }
    return maxVal;
}

int main() {
    std::cout << "Factorial of 5: " << factorial(5) << std::endl;
    
    std::vector<int> numbers = {3, 7, 2, 9, 1};
    std::cout << "Maximum: " << findMax(numbers) << std::endl;
    
    return 0;
}
