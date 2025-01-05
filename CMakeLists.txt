cmake_minimum_required(VERSION 3.10)

# ----------------------------------------------------
#  Project Setup
# ----------------------------------------------------
project(uncertainties
    VERSION 0.1.0
    DESCRIPTION "C++ library providing a ufloat type for uncertainty propagation"
    LANGUAGES CXX
)

# Use C++17 (or another standard of your choice)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ----------------------------------------------------
#  Build Options
# ----------------------------------------------------
option(UNCERTAINTIES_BUILD_TESTS    "Build unit tests"         ON)
option(UNCERTAINTIES_BUILD_EXAMPLES "Build example programs"   ON)

# ----------------------------------------------------
#  Library Target
# ----------------------------------------------------
# Create a library named 'uncertainties' from your ufloat.cpp source
add_library(uncertainties
    src/ufloat.cpp
)

# Let users #include "uncertainties/ufloat.hpp" from <project>/include
target_include_directories(uncertainties
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# ----------------------------------------------------
#  (Optional) Unit Tests
# ----------------------------------------------------
if (UNCERTAINTIES_BUILD_TESTS)
    enable_testing()

    # Example: Google Test integration (only if GTest is available).
    # You can also use Catch2, doctest, etc. in a similar way.
    find_package(GTest CONFIG QUIET)
    if (GTest_FOUND)
        add_executable(test_ufloat tests/test_ufloat.cpp)
        target_link_libraries(test_ufloat PRIVATE
            GTest::gtest_main
            uncertainties
        )
        add_test(NAME test_ufloat COMMAND test_ufloat)
        # Custom target to run tests automatically after build
        add_custom_target(run_tests ALL
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -j ${CMAKE_BUILD_PARALLEL_LEVEL}
        DEPENDS test_ufloat
        )
    else()
        message(STATUS "GTest not found. No tests will be built.")
    endif()
endif()

# ----------------------------------------------------
#  (Optional) Examples
# ----------------------------------------------------
if (UNCERTAINTIES_BUILD_EXAMPLES)
    add_executable(example_basic examples/example_basic.cpp)
    target_link_libraries(example_basic PRIVATE uncertainties)
endif()

# ----------------------------------------------------
#  Installation Instructions
# ----------------------------------------------------
# Install the library (static or shared)
install(TARGETS uncertainties
    EXPORT uncertainties-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install public headers: everything under include/uncertainties
install(
    DIRECTORY include/
    DESTINATION include
)

# Export configuration for find_package(uncertainties)
install(EXPORT uncertainties-targets
    FILE uncertaintiesConfig.cmake
    NAMESPACE uncertainties::
    DESTINATION lib/cmake/uncertainties
)

# ----------------------------------------------------
#  Summary
# ----------------------------------------------------
# After configuring and building:
#   - 'uncertainties' library will be built.
#   - Any tests (if GTest found) can be run with `ctest`.
#   - The library and headers can be installed with `make install` (or `cmake --build . --target install`).
#
# Example usage in another project:
#   find_package(uncertainties CONFIG REQUIRED)
#   target_link_libraries(my_app PRIVATE uncertainties::uncertainties)
#
# That's it!
