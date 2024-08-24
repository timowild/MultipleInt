#!/usr/bin/env bash
set -o xtrace

compiler_output_dir="clang-release"

benchmark_dir="../build/${compiler_output_dir}/benchmark"

repdir="${benchmark_dir}/results/reports"
timedir="${benchmark_dir}/results/time-vizes"
bwdir="${benchmark_dir}/results/bw-vizes"

# shellcheck source=/dev/null
# source .venv/bin/activate

for benchmark in "${repdir}/$1"*.json; do
    bname=$(basename "${benchmark}")
    python plot.py "${benchmark}" "${timedir}"/"${bname}".png
    python bandwidth.py "${benchmark}" "${bwdir}"/"${bname}".png
done
