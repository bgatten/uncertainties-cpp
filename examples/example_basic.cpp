// example_basic.cpp
#include <iostream>             // for std::cout
#include "uncertainties/udouble.hpp"  // your header for udouble

int main() {
    // Create two udouble objects with nominal values and standard deviations
    uncertainties::udouble a(1.0, 0.1); // 1.0 ± 0.1
    uncertainties::udouble b(2.0, 0.2); // 2.0 ± 0.2

    // Perform arithmetic with automatic error propagation
    uncertainties::udouble c = a + b; // (1.0 + 2.0) ± sqrt(0.1^2 + 0.2^2)

    // Print the result: "3.0 ± 0.223606..." (depending on floating-point precision)
    std::cout << "c = " << c << std::endl;

    // For demonstration, let's also multiply them:
    uncertainties::udouble d = a * b; 
    // Nominal = 1.0 * 2.0 = 2.0
    // Stddev  = sqrt((2.0 * 0.1)^2 + (1.0 * 0.2)^2) = sqrt(0.04 + 0.04) = 0.282843...

    std::cout << "d = " << d << std::endl;

    return 0;
}
