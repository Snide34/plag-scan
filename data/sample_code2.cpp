#include <iostream>
#include <vector>

// Calculate factorial recursively
int computeFactorial(int num) {
    if (num <= 1) {
        return 1;
    }
    return num * computeFactorial(num - 1);
}

// Find the largest element in array
int getMaximum(const std::vector<int>& data) {
    int maximum = data[0];
    for (size_t idx = 1; idx < data.size(); ++idx) {
        if (data[idx] > maximum) {
            maximum = data[idx];
        }
    }
    return maximum;
}

int main() {
    std::cout << "Factorial of 5: " << computeFactorial(5) << std::endl;
    
    std::vector<int> values = {3, 7, 2, 9, 1};
    std::cout << "Max value: " << getMaximum(values) << std::endl;
    
    return 0;
}
