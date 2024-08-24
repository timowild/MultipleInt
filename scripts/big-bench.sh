#!/usr/bin/env bash
set -o xtrace

compiler_output_dir="clang-release"

benchmark_dir="../build/${compiler_output_dir}/benchmark"

repdir="${benchmark_dir}/results/reports"
timedir="${benchmark_dir}/results/time-vizes"
bwdir="${benchmark_dir}/results/bw-vizes"

mkdir -p "${repdir}" "${bwdir}" "${timedir}"

while IFS=';' read -ra BENCHMARKS; do
    for benchmark in "${BENCHMARKS[@]}"; do
        ${benchmark_dir}/$2 \
            --benchmark_filter="_warmup_|${benchmark}" \
            --benchmark_out="${repdir}"/"${benchmark}".json \
            --benchmark_repetitions=3 \
            --benchmark_report_aggregates_only=true \
            --benchmark_out_format=json 
    done   
done <<< $1