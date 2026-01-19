# Uncertainties Library

This project provides a C++ implementation of a `udouble` class for handling values with uncertainties, similar to Python's `uncertainties` package. The `udouble` class supports automatic error propagation for arithmetic operations.

## Features

- Create objects with nominal values and standard deviations.
- Implicit conversion from `double` (zero uncertainty).
- Arithmetic operations (`+`, `-`, `*`, `/`) with automatic error propagation.
- Unary operators (`+`, `-`).
- Compound assignment operators (`+=`, `-=`, `*=`, `/=`).
- Comparison operators (`==`, `!=`, `<`, `>`, `<=`, `>=`) based on nominal values.
- Mixed-type operations between `udouble` and `double`.
- Exponentiation with `pow()`.
- Mathematical functions:
  - Trigonometric: `sin()`, `cos()`, `tan()`
  - Inverse trigonometric: `asin()`, `acos()`, `atan()`, `atan2()`
  - Hyperbolic: `sinh()`, `cosh()`, `tanh()`
  - Inverse hyperbolic: `asinh()`, `acosh()`, `atanh()`
  - Exponential/logarithmic: `exp()`, `log()`, `log10()`, `sqrt()`
  - Other: `abs()`, `hypot()`
- Multiple output formats: default, scientific notation, compact notation.
- `constexpr` support for compile-time evaluation.
- Includes unit tests and examples.

## Installation

### Prerequisites

1. **C++17 or later**
2. **CMake 3.10 or later**
3. Optional: [Google Test (GTest)](https://github.com/google/googletest) for unit testing.

### Build Instructions

1. Clone the repository:
   ```bash
   git clone <repository_url>
   cd uncertainties-cpp
   ```

2. Create a build directory and configure the project:
   ```bash
   mkdir build && cd build
   cmake ..
   ```

3. Build the project:
   ```bash
   cmake --build .
   ```

4. Optionally, install the library:
   ```bash
   cmake --build . --target install
   ```

## Running Tests

1. Ensure tests are enabled in the build configuration:
   ```bash
   cmake -DUNCERTAINTIES_BUILD_TESTS=ON ..
   ```

2. Build and run tests:
   ```bash
   cmake --build .
   ctest --verbose
   ```

Alternatively, run tests automatically during the build process by running:
```bash
cmake --build . --target run_tests
```

## Examples

### Example: Basic Usage

Here’s an example of how to use the `udouble` class:

```cpp
#include <iostream>
#include "uncertainties/udouble.hpp"

int main() {
    uncertainties::udouble a(1.0, 0.1); // 1.0 ± 0.1
    uncertainties::udouble b(2.0, 0.2); // 2.0 ± 0.2

    uncertainties::udouble c = a + b; // 3.0 ± sqrt(0.1^2 + 0.2^2)

    std::cout << "c = " << c << std::endl; // Output: 3.0 ± 0.223606

    return 0;
}
```

### Example: Mathematical Functions

The library provides mathematical functions with automatic error propagation:

```cpp
#include <iostream>
#include "uncertainties/udouble.hpp"
#include "uncertainties/umath.hpp"

int main() {
    uncertainties::udouble x(1.0, 0.1);

    // Trigonometric functions
    uncertainties::udouble s = uncertainties::sin(x);
    uncertainties::udouble c = uncertainties::cos(x);
    uncertainties::udouble t = uncertainties::tan(x);

    // Inverse trigonometric functions
    uncertainties::udouble as = uncertainties::asin(uncertainties::udouble(0.5, 0.01));
    uncertainties::udouble at = uncertainties::atan(x);

    // Hyperbolic functions
    uncertainties::udouble sh = uncertainties::sinh(x);
    uncertainties::udouble ch = uncertainties::cosh(x);

    // Exponential and logarithmic functions
    uncertainties::udouble e = uncertainties::exp(x);
    uncertainties::udouble l = uncertainties::log(x);
    uncertainties::udouble sq = uncertainties::sqrt(uncertainties::udouble(4.0, 0.1));

    // Exponentiation
    uncertainties::udouble p = uncertainties::pow(x, uncertainties::udouble(2.0, 0.0));

    std::cout << "sin(x) = " << s << std::endl;
    std::cout << "sqrt(4) = " << sq << std::endl;

    return 0;
}
```

### Example: Operators

The library supports various operators for convenient calculations:

```cpp
#include <iostream>
#include "uncertainties/udouble.hpp"

int main() {
    uncertainties::udouble a(10.0, 0.5);
    uncertainties::udouble b(3.0, 0.2);

    // Unary operators
    uncertainties::udouble neg = -a;  // -10.0 ± 0.5

    // Compound assignment
    uncertainties::udouble c = a;
    c += b;  // 13.0 ± sqrt(0.5² + 0.2²)
    c *= 2.0;  // Scale by constant

    // Comparison (based on nominal values)
    if (a > b) {
        std::cout << "a is greater than b" << std::endl;
    }

    std::cout << "c = " << c << std::endl;

    return 0;
}
```

### Example: Implicit Conversion

Plain `double` values are automatically converted to `udouble` with zero uncertainty:

```cpp
#include <iostream>
#include "uncertainties/udouble.hpp"

int main() {
    uncertainties::udouble a(5.0, 0.1);

    // Implicit conversion from double
    uncertainties::udouble b = 3.0;  // 3.0 ± 0.0

    // Works in arithmetic too
    uncertainties::udouble c = a + 2.0;  // Adds constant with no additional uncertainty

    // Can pass doubles to functions expecting udouble
    auto square = [](const uncertainties::udouble& x) { return x * x; };
    uncertainties::udouble d = square(4.0);  // 16.0 ± 0.0

    return 0;
}
```

### Example: Output Formatting

The library provides multiple ways to format output:

```cpp
#include <iostream>
#include "uncertainties/udouble.hpp"

int main() {
    uncertainties::udouble x(1.23456, 0.00789);

    // Default stream output
    std::cout << x << std::endl;  // 1.23456 ± 0.00789

    // Custom precision
    std::cout << x.to_string(3) << std::endl;  // 1.23 ± 0.00789

    // Scientific notation
    std::cout << x.to_scientific(2) << std::endl;  // 1.23e+00 ± 7.89e-03

    // Compact notation (uncertainty in parentheses)
    std::cout << x.to_compact() << std::endl;  // 1.235(79)

    return 0;
}
```

### Build and Run Example

1. Enable examples in the build configuration:
   ```bash
   cmake -DUNCERTAINTIES_BUILD_EXAMPLES=ON ..
   ```

2. Build and run the example:
   ```bash
   cmake --build .
   ./example_basic
   ```

## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository.
2. Create a new branch.
3. Commit your changes.
4. Open a pull request.

## License

This project is licensed under the BSD 3-Clause License. See the `LICENSE` file for details.

---

Happy coding!

