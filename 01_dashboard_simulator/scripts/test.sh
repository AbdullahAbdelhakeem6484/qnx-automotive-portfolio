#!/bin/bash
# ===============================================================================
# QNX Automotive Dashboard Simulator - Test Script
# ===============================================================================
# Description: Comprehensive testing script for the dashboard simulator
# Usage: ./scripts/test.sh [test_type]
# ===============================================================================

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test configuration
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_DIR/build"
BIN_DIR="$BUILD_DIR/bin"
TEST_LOG="$BUILD_DIR/test_results.log"

# Test counters
TESTS_PASSED=0
TESTS_FAILED=0
TESTS_TOTAL=0

# Functions
print_header() {
    echo -e "${BLUE}===============================================================================${NC}"
    echo -e "${BLUE}  QNX Dashboard Simulator Test Suite${NC}"
    echo -e "${BLUE}===============================================================================${NC}"
    echo "Project Directory: $PROJECT_DIR"
    echo "Build Directory: $BUILD_DIR"
    echo "Test Log: $TEST_LOG"
    echo ""
}

print_test_result() {
    local test_name="$1"
    local result="$2"
    local message="$3"
    
    TESTS_TOTAL=$((TESTS_TOTAL + 1))
    
    if [ "$result" = "PASS" ]; then
        echo -e "${GREEN}[PASS]${NC} $test_name"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}[FAIL]${NC} $test_name: $message"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
    
    echo "$(date): [$result] $test_name - $message" >> "$TEST_LOG"
}

# Test 1: Project Structure Validation
test_project_structure() {
    echo -e "${YELLOW}Running Project Structure Tests...${NC}"
    
    # Check required directories
    local dirs=("src" "include" "resources" "build" "scripts")
    for dir in "${dirs[@]}"; do
        if [ -d "$PROJECT_DIR/$dir" ]; then
            print_test_result "Directory $dir exists" "PASS" ""
        else
            print_test_result "Directory $dir exists" "FAIL" "Directory not found"
        fi
    done
    
    # Check required files
    local files=(
        "Makefile"
        "README.md"
        "ARCHITECTURE.md"
        "include/dashboard.h"
        "include/gui_manager.h"
        "include/data_simulator.h"
        "src/main.c"
        "src/gui_manager.c"
        "src/data_simulator.c"
        "resources/config/dashboard.conf"
    )
    
    for file in "${files[@]}"; do
        if [ -f "$PROJECT_DIR/$file" ]; then
            print_test_result "File $file exists" "PASS" ""
        else
            print_test_result "File $file exists" "FAIL" "File not found"
        fi
    done
}

# Test 2: Build System Tests
test_build_system() {
    echo -e "${YELLOW}Running Build System Tests...${NC}"
    
    cd "$PROJECT_DIR"
    
    # Test clean build
    if make clean > /dev/null 2>&1; then
        print_test_result "Make clean" "PASS" ""
    else
        print_test_result "Make clean" "FAIL" "Clean failed"
    fi
    
    # Test debug build
    if make debug > /dev/null 2>&1; then
        print_test_result "Debug build" "PASS" ""
    else
        print_test_result "Debug build" "FAIL" "Debug build failed"
    fi
    
    # Test release build
    if make release > /dev/null 2>&1; then
        print_test_result "Release build" "PASS" ""
    else
        print_test_result "Release build" "FAIL" "Release build failed"
    fi
    
    # Check if binary was created
    if [ -f "$BIN_DIR/qnx-dashboard-simulator-debug-x86_64" ]; then
        print_test_result "Binary creation" "PASS" ""
    else
        print_test_result "Binary creation" "FAIL" "Binary not found"
    fi
}

# Test 3: Code Quality Tests
test_code_quality() {
    echo -e "${YELLOW}Running Code Quality Tests...${NC}"
    
    cd "$PROJECT_DIR"
    
    # Check for basic syntax errors (using gcc if available)
    if command -v gcc >/dev/null 2>&1; then
        if gcc -fsyntax-only -I./include src/*.c > /dev/null 2>&1; then
            print_test_result "Syntax check" "PASS" ""
        else
            print_test_result "Syntax check" "FAIL" "Syntax errors found"
        fi
    else
        print_test_result "Syntax check" "SKIP" "GCC not available"
    fi
    
    # Check for TODO/FIXME comments
    local todo_count=$(grep -r "TODO\|FIXME" src/ include/ 2>/dev/null | wc -l)
    if [ "$todo_count" -eq 0 ]; then
        print_test_result "Code completeness" "PASS" ""
    else
        print_test_result "Code completeness" "WARN" "$todo_count TODO/FIXME items found"
    fi
    
    # Check file permissions
    if [ -x "$PROJECT_DIR/scripts/test.sh" ]; then
        print_test_result "Script permissions" "PASS" ""
    else
        print_test_result "Script permissions" "FAIL" "Test script not executable"
    fi
}

# Test 4: Configuration Tests
test_configuration() {
    echo -e "${YELLOW}Running Configuration Tests...${NC}"
    
    local config_file="$PROJECT_DIR/resources/config/dashboard.conf"
    
    # Check configuration file exists and is readable
    if [ -r "$config_file" ]; then
        print_test_result "Config file readable" "PASS" ""
    else
        print_test_result "Config file readable" "FAIL" "Cannot read config file"
        return
    fi
    
    # Check for required configuration sections
    local sections=("display" "performance" "simulation" "safety" "theme")
    for section in "${sections[@]}"; do
        if grep -q "^\[$section\]" "$config_file"; then
            print_test_result "Config section [$section]" "PASS" ""
        else
            print_test_result "Config section [$section]" "FAIL" "Section not found"
        fi
    done
    
    # Validate configuration values
    if grep -q "window_width = 1024" "$config_file"; then
        print_test_result "Config validation" "PASS" ""
    else
        print_test_result "Config validation" "FAIL" "Invalid config values"
    fi
}

# Test 5: Documentation Tests
test_documentation() {
    echo -e "${YELLOW}Running Documentation Tests...${NC}"
    
    # Check README completeness
    local readme="$PROJECT_DIR/README.md"
    if [ -f "$readme" ]; then
        if grep -q "Installation" "$readme" && grep -q "Usage" "$readme"; then
            print_test_result "README completeness" "PASS" ""
        else
            print_test_result "README completeness" "FAIL" "Missing required sections"
        fi
    else
        print_test_result "README exists" "FAIL" "README.md not found"
    fi
    
    # Check architecture documentation
    local arch_doc="$PROJECT_DIR/ARCHITECTURE.md"
    if [ -f "$arch_doc" ]; then
        if grep -q "Architecture" "$arch_doc"; then
            print_test_result "Architecture documentation" "PASS" ""
        else
            print_test_result "Architecture documentation" "FAIL" "Incomplete architecture docs"
        fi
    else
        print_test_result "Architecture documentation" "FAIL" "ARCHITECTURE.md not found"
    fi
    
    # Check code comments
    local comment_count=$(grep -r "^[[:space:]]*\*\|^[[:space:]]*//" src/ include/ 2>/dev/null | wc -l)
    if [ "$comment_count" -gt 50 ]; then
        print_test_result "Code documentation" "PASS" "$comment_count comment lines found"
    else
        print_test_result "Code documentation" "WARN" "Only $comment_count comment lines found"
    fi
}

# Test 6: Runtime Tests (if binary exists)
test_runtime() {
    echo -e "${YELLOW}Running Runtime Tests...${NC}"
    
    local binary="$BIN_DIR/qnx-dashboard-simulator-debug-x86_64"
    
    if [ ! -f "$binary" ]; then
        print_test_result "Binary availability" "FAIL" "Binary not found for runtime tests"
        return
    fi
    
    # Test help option
    if timeout 5s "$binary" --help > /dev/null 2>&1; then
        print_test_result "Help option" "PASS" ""
    else
        print_test_result "Help option" "FAIL" "Help option failed or timed out"
    fi
    
    # Test version output
    if "$binary" --version > /dev/null 2>&1; then
        print_test_result "Version option" "PASS" ""
    else
        print_test_result "Version option" "SKIP" "Version option not implemented"
    fi
}

# Test Summary
print_summary() {
    echo ""
    echo -e "${BLUE}===============================================================================${NC}"
    echo -e "${BLUE}  Test Summary${NC}"
    echo -e "${BLUE}===============================================================================${NC}"
    echo "Total Tests: $TESTS_TOTAL"
    echo -e "Passed: ${GREEN}$TESTS_PASSED${NC}"
    echo -e "Failed: ${RED}$TESTS_FAILED${NC}"
    
    if [ $TESTS_FAILED -eq 0 ]; then
        echo -e "${GREEN}All tests passed!${NC}"
        exit 0
    else
        echo -e "${RED}Some tests failed. Check the log for details.${NC}"
        exit 1
    fi
}

# Main execution
main() {
    # Initialize log file
    mkdir -p "$BUILD_DIR"
    echo "=== QNX Dashboard Simulator Test Run - $(date) ===" > "$TEST_LOG"
    
    print_header
    
    # Run test suites
    test_project_structure
    test_build_system
    test_code_quality
    test_configuration
    test_documentation
    test_runtime
    
    print_summary
}

# Handle command line arguments
case "${1:-all}" in
    "structure")
        test_project_structure
        ;;
    "build")
        test_build_system
        ;;
    "quality")
        test_code_quality
        ;;
    "config")
        test_configuration
        ;;
    "docs")
        test_documentation
        ;;
    "runtime")
        test_runtime
        ;;
    "all"|*)
        main
        ;;
esac 