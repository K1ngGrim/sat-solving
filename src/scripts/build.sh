#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

# Pull the CaDiCaL solver submodule.
cd "$PROJECT_ROOT"
git submodule update --init --recursive

# Build the CaDiCaL solver library.
#cd "$PROJECT_ROOT/cadical"
#./configure
#make

# Configure and build own code.
cd "$PROJECT_ROOT/.."
cmake -S . -B .
cmake --build . --target blatt_1_1
