#!/usr/bin/env bash
# Exit on error
set -o errexit

# Install python dependencies
pip install -r requirements.txt

# Compile C++ backend to Linux binary
g++ backend/main.cpp -o backend/main
