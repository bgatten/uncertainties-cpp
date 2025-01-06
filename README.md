# Uncertainties Library

This project provides a C++ implementation of a `udouble` class for handling values with uncertainties, similar to Python's `uncertainties` package. The `udouble` class supports automatic error propagation for arithmetic operations.

## Features

- Create objects with nominal values and standard deviations.
- Perform arithmetic operations (`+`, `-`, `*`, `/`) with automatic error propagation.
- Output values in the format "value ± uncertainty".
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
   cd uncertainties
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

