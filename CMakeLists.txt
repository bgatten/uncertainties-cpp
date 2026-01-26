cmake_minimum_required(VERSION 3.10)

# ----------------------------------------------------
#  Project Setup
# ----------------------------------------------------
project(uncertainties
    VERSION 0.2.0
    DESCRIPTION "C++ library providing a udouble type for uncertainty propagation"
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
option(UNCERTAINTIES_BUILD_DOCS     "Build documentation"      OFF)

# ----------------------------------------------------
#  Library Target
# ----------------------------------------------------
# Create a library named 'uncertainties' from your udouble.cpp source
add_library(uncertainties
    src/udouble.cpp
    src/umath.cpp
)

# Let users #include "uncertainties/udouble.hpp" from <project>/include
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

    # Google Test integration (only if GTest is available)
    find_package(GTest CONFIG QUIET)
    # Eigen integration (optional)
    find_package(Eigen3 CONFIG QUIET)

    if (GTest_FOUND)
        # Core tests
        add_executable(test_udouble tests/test_udouble.cpp)
        add_executable(test_umath tests/test_umath.cpp)
        add_executable(test_correlation tests/test_correlation.cpp)
        target_link_libraries(test_udouble PRIVATE
            GTest::gtest_main
            uncertainties
        )
        target_link_libraries(test_umath PRIVATE
            GTest::gtest_main
            uncertainties
        )
        target_link_libraries(test_correlation PRIVATE
            GTest::gtest_main
            uncertainties
        )
        add_test(NAME test_udouble COMMAND test_udouble)
        add_test(NAME test_umath COMMAND test_umath)
        add_test(NAME test_correlation COMMAND test_correlation)

        # Eigen tests (only if Eigen is available)
        set(TEST_TARGETS test_udouble test_umath test_correlation)
        if (Eigen3_FOUND)
            add_executable(test_eigen tests/test_eigen.cpp)
            target_link_libraries(test_eigen PRIVATE
                GTest::gtest_main
                uncertainties
                Eigen3::Eigen
            )
            add_test(NAME test_eigen COMMAND test_eigen)
            list(APPEND TEST_TARGETS test_eigen)
            message(STATUS "Eigen found. Eigen integration tests will be built.")
        else()
            message(STATUS "Eigen not found. Eigen integration tests will be skipped.")
        endif()

        # Custom target to run tests automatically after build
        add_custom_target(run_tests ALL
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -j ${CMAKE_BUILD_PARALLEL_LEVEL}
            DEPENDS ${TEST_TARGETS}
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
#  (Optional) Documentation
# ----------------------------------------------------
if (UNCERTAINTIES_BUILD_DOCS)
    find_package(Doxygen QUIET)
    if (DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
        )
        message(STATUS "Doxygen found. Use 'make docs' to build documentation.")
    else()
        message(STATUS "Doxygen not found. Documentation will not be built.")
    endif()
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
