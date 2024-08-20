#!/usr/bin/env bash
find ./include ./test ./benchmark -iname "*.hpp" -o -iname "*.cpp" | xargs clang-format -i
