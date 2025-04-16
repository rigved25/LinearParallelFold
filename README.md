# LinearFold-Parallel: Usage Guide

## Build Instructions

### Build the Parallel Version
```bash
make lfomp
```

This builds the parallel version using:
```bash
g++ src/LinearFold.cpp -std=c++11 -O3 -fopenmp -Dlv -Dis_cube_pruning -Dis_candidate_list -o bin_omp/linearfold_v
```

### Build the Serial Version
```bash
make linearfold
```

## Testing Correctness

### Run Serial Version
```bash
cat testseq | ./linearfold
```

### Run Parallel Version
```bash
cat testseq | ./linearfold --omp
```

## Benchmarking

To benchmark across multiple sequences from the `testseq` file:
```bash
./compare.sh
```

To change the beam size, edit the `compare.sh` script directly.

## Monitoring Threads

Use the `top` command while running the parallel version to observe CPU usage and thread activity:
```bash
top
```
