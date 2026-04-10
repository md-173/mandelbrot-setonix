# Parallel Mandelbrot Generation on Setonix

A high-performance computing (HPC) project focused on the parallelization of the Mandelbrot set using C and the Message Passing Interface (MPI). This project was developed and benchmarked on the **Setonix supercomputer** at the Pawsey Supercomputing Research Centre.

## Overview

The primary objective of this project was to analyze performance gains achieved by transitioning from serial algorithms to various parallel distributed computing strategies. By comparing block, cyclic, and dynamic workload distribution, this project identifies the most efficient methods for handling the irregular computational density inherent in fractal generation.

## Algorithms Implemented

This project explores four distinct computational strategies:

1. **Serial Baseline:** A single-threaded implementation used to establish a performance benchmark.
2. **Static Block Partitioning:** Divides the fractal into contiguous blocks. While simple to implement, this often leads to load imbalance due to the varying complexity of different fractal regions.
3. **Cyclic Partitioning:** A static interleaving approach where rows are distributed in a round-robin fashion ($i, i+P, i+2P \dots$). This significantly improves load balancing over block partitioning with minimal overhead.
4. **Dynamic Parallel (Master-Worker):** A centralized load-balancing strategy where a master node assigns tasks to workers on-demand, ensuring maximum node utilization at the cost of increased communication overhead.

## Key Features

* **Parallel Processing:** Implemented using MPI for robust inter-process communication.
* **HPC Optimized:** Specifically tuned for the Setonix (Cray EX) architecture.
* **Performance Analysis:** Integrated timing logic to calculate speedup, efficiency, and communication-to-computation ratios.
* **Workload Benchmarking:** Designed to stress-test node utilization across thousands of concurrent iterations.

## Tech Stack

* **Language:** C
* **Parallel Framework:** MPI (Message Passing Interface)
* **Environment:** Setonix (Pawsey Supercomputing Research Centre)

## Performance Metrics

The project includes a comprehensive analysis of:
* **Execution Time:** A four-way comparison across all implemented algorithms.
* **Speedup Factor:** Measuring the ratio of serial execution time to parallel execution time ($S_p = T_1 / T_p$).
* **Load Balance Efficiency:** Evaluating how Cyclic and Dynamic strategies mitigate the "idle-node" problem found in Block partitioning.

## Getting Started

### Prerequisites

* Access to an HPC environment with MPI (e.g., OpenMPI, MPICH, or Cray-MPICH).
* GCC or a similar C compiler.

### Compilation

Compile the desired implementation using `mpicc`. For example, to compile the Cyclic version:
```bash
mpicc -o mandelbrot_cp mandelbrot_cp_timed.c -lm
```

### Execution

To run the program with 16 processes on a cluster:
```bash
mpirun -np 16 ./mandelbrot_cp
```

## Project Structure

* **`mandelbrot_serial_timed.c`**: Baseline serial implementation.
* **`mandelbrot_bp_timed.c`**: Static Block Parallel implementation.
* **`mandelbrot_cp_timed.c`**: Cyclic Partitioning implementation.
* **`mandelbrot_dyn_timed.c`**: Dynamic Master-Worker implementation.
* **`Performance_Analysis_Parallel_Mandelbrot_Generation.pdf`**: Full technical report and performance results.
* **`slurm_script.sh`**: Job submission script for the Setonix cluster.
