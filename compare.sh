#!/bin/bash

# CONFIG
RUNS=20
SEQFILE="./testseq"
WRAPPER="./linearfold"
CSV="benchmark_results.csv"

# OPTIONS
SERIAL_OPTS="--beamsize 0"
PARALLEL_OPTS="--beamsize 0 --omp"

# Init CSV
echo "Sequence,Length,Serial Avg Time,Parallel Avg Time,Speedup" > $CSV

# Loop over sequences
while IFS= read -r SEQ; do
    if [[ -z "$SEQ" || "$SEQ" == \>* ]]; then continue; fi

    echo "Benchmarking: ${SEQ:0:30}... (len=${#SEQ})"

    TMP_SERIAL="serial.tmp"
    TMP_PARALLEL="parallel.tmp"
    > $TMP_SERIAL
    > $TMP_PARALLEL

    # SERIAL runs
    for i in $(seq 1 $RUNS); do
        TIME=$(echo "$SEQ" | $WRAPPER $SERIAL_OPTS | grep "Total runtime" | awk '{print $3}')
        echo "$TIME" >> $TMP_SERIAL
    done

    # PARALLEL runs
    for i in $(seq 1 $RUNS); do
        TIME=$(echo "$SEQ" | $WRAPPER $PARALLEL_OPTS | grep "Total runtime" | awk '{print $3}')
        echo "$TIME" >> $TMP_PARALLEL
    done

    # Averages and Speedup
    AVG_SERIAL=$(awk '{sum+=$1} END {printf "%.6f", sum/NR}' $TMP_SERIAL)
    AVG_PARALLEL=$(awk '{sum+=$1} END {printf "%.6f", sum/NR}' $TMP_PARALLEL)
    SPEEDUP=$(awk -v s=$AVG_SERIAL -v p=$AVG_PARALLEL 'BEGIN {printf "%.2fx", s/p}')

    echo "  Serial avg: $AVG_SERIAL s | Parallel avg: $AVG_PARALLEL s | Speedup: $SPEEDUP"
    echo "\"$SEQ\",${#SEQ},$AVG_SERIAL,$AVG_PARALLEL,$SPEEDUP" >> $CSV

    rm -f $TMP_SERIAL $TMP_PARALLEL
    echo ""
done < "$SEQFILE"
