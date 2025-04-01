#!/bin/bash
# scripts/run_tests.sh

# Make the script exit if any command fails
set -e

# Get the path to the test executable from the first argument
TEST_EXECUTABLE="${1:-./tests/websocket_test}"

echo "Running WebSocket tests..."

# Run the tests with XML output for CI integration
if [ -z "$TEST_EXECUTABLE" ]; then
    echo "Error: No test executable specified"
    exit 1
fi

"${TEST_EXECUTABLE}" --gtest_output=xml:test_results.xml


# Check if tests passed
if [ $? -eq 0 ]; then
  echo "All tests passed!"
  exit 0
else
  echo "Tests failed!"
  exit 1
fi
