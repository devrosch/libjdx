#!/usr/bin/env bash

# Capture code coverage. If code coverage is less than $COVERAGE_THRESHOLD, return error.

set -eo pipefail

make coverage-capture

make coverage-report \
    | tee /dev/stderr \
    | grep -E 'lines\.{6,}:' \
    | awk -F': |%' '{print $2}' \
    | { read coverage; echo "=== line coverage: ${coverage}%, threshold: ${COVERAGE_THRESHOLD}% ==="; (( $(echo "$coverage >= $COVERAGE_THRESHOLD" | bc -l) )); }
