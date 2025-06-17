#!/bin/bash
# File: run_tests.sh - Enhanced Test Runner with Overhead Analysis

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
PURPLE='\033[0;35m'
BOLD_WHITE='\033[1;37m'
CYAN='\033[0;36m'
NC='\033[0m'

echo -e "${PURPLE}🚀 Daedalus Test Suite Runner${NC}"
echo -e "${CYAN}Enhanced with High-Resolution Performance Analysis${NC}"
echo "=================================================="

# Build test dependencies
echo -e "${BLUE}🔧 Building test dependencies...${NC}"
build_start=$(date +%s.%N)
if ! make -s always > /dev/null 2>&1; then
    echo -e "${RED}❌ Failed to build test dependencies${NC}"
    exit 1
fi
build_end=$(date +%s.%N)
build_time=$(echo "$build_end - $build_start" | bc -l)
echo -e "${BOLD_WHITE}   Build completed in $(printf "%.6f" $build_time) seconds${NC}"

# Counter variables
TOTAL_FILES=0
PASSED_FILES=0
FAILED_FILES=()
TOTAL_TEST_TIME=0
TOTAL_SHELL_OVERHEAD=0

# Function to run a test
run_test() {
    local test_name="$1"
    local make_target="$2"

    echo -ne "${BLUE}📋 Running $test_name...${NC}"

    TOTAL_FILES=$((TOTAL_FILES + 1))

    # Capture start time for full process
    process_start=$(date +%s.%N)

    # Run the test and capture both exit code and output
    if test_output=$(make -s "$make_target" 2>&1); then
        process_end=$(date +%s.%N)

        # Extract timing from the test output if available
        actual_test_time=$(echo "$test_output" | grep -o "Total Test Time: [0-9]*\.[0-9]*" | grep -o "[0-9]*\.[0-9]*" || echo "0.000000")

        process_elapsed=$(echo "$process_end - $process_start" | bc -l)
        shell_overhead=$(echo "$process_elapsed - $actual_test_time" | bc -l)

        TOTAL_TEST_TIME=$(echo "$TOTAL_TEST_TIME + $actual_test_time" | bc -l)
        TOTAL_SHELL_OVERHEAD=$(echo "$TOTAL_SHELL_OVERHEAD + $shell_overhead" | bc -l)

        echo -e " ${BOLD_WHITE}(Test: $(printf "%.6f" $actual_test_time)s | Shell: $(printf "%.3f" $shell_overhead)s)${NC}"
        echo -e "  ${GREEN}✅ $test_name: All tests passed${NC}"
        PASSED_FILES=$((PASSED_FILES + 1))
    else
        process_end=$(date +%s.%N)
        process_elapsed=$(echo "$process_end - $process_start" | bc -l)
        TOTAL_SHELL_OVERHEAD=$(echo "$TOTAL_SHELL_OVERHEAD + $process_elapsed" | bc -l)

        echo -e " ${BOLD_WHITE}(Process: $(printf "%.6f" $process_elapsed)s)${NC}"
        echo -e "  ${RED}❌ $test_name: Some tests failed${NC}"
        FAILED_FILES+=("$test_name:$make_target")
    fi
}

# Record overall start time
overall_start=$(date +%s.%N)

# Run all tests
echo ""
run_test "Input Basic" "run-test-input-basic"
run_test "Input Advanced" "run-test-input-advanced"
run_test "Input Edge Cases" "run-test-input-edge"
run_test "Input Performance" "run-test-input-performance"
run_test "Input Errors" "run-test-input-errors"
run_test "Input Integration" "run-test-input-integration"

# Calculate overall execution time
overall_end=$(date +%s.%N)
overall_time=$(echo "$overall_end - $overall_start" | bc -l)

# Final Summary
FAILED_COUNT=$((TOTAL_FILES - PASSED_FILES))

echo ""
echo -e "${PURPLE}📊 Global Test Results Summary${NC}"
echo "=============================================="
echo -e "Test Files Run: ${YELLOW}$TOTAL_FILES${NC}"
echo -e "✅ Passed:      ${GREEN}$PASSED_FILES${NC}"
echo -e "❌ Failed:      ${RED}$FAILED_COUNT${NC}"
echo ""
echo -e "${CYAN}⚡ Performance Analysis:${NC}"
echo -e "${BOLD_WHITE}⏱️  Pure Test Time:    $(printf "%10.5f" $TOTAL_TEST_TIME) seconds${NC}"
echo -e "${BOLD_WHITE}🐚 Shell Overhead:     $(printf "%10.3f" $TOTAL_SHELL_OVERHEAD) seconds${NC}"
echo -e "${BOLD_WHITE}🕒 Total Runtime:      $(printf "%10.4f" $overall_time) seconds${NC}"

# Calculate efficiency percentage
if (( $(echo "$overall_time > 0" | bc -l) )); then
    efficiency=$(echo "scale=2; $TOTAL_TEST_TIME * 100 / $overall_time" | bc -l)
    echo -e "${BOLD_WHITE}📈 Test Efficiency:    $(printf "%10.2f" $efficiency)%${NC}"
fi

if [ ${#FAILED_FILES[@]} -eq 0 ]; then
    echo ""
    echo -e "${GREEN}🎉 All tests passed across all files! Excellent work!${NC}"
    echo -e "${CYAN}💡 For precise individual test timing, run tests directly with 'make run-test-*'${NC}"
    exit 0
else
    echo ""
    echo -e "${RED}💥 Some tests failed in the following files:${NC}"
    for failed in "${FAILED_FILES[@]}"; do
        name=$(echo "$failed" | cut -d':' -f1)
        target=$(echo "$failed" | cut -d':' -f2)
        echo -e "  ${RED}• $name${NC}"
        echo -e "    To see detailed failures, run: ${YELLOW}make $target${NC}"
    done
    echo ""
    echo -e "${YELLOW}💡 Tip: Run individual test files to see specific function failures${NC}"
    exit 1
fi
