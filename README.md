# Parallel Mandelbrot Generation on Setonix

A high-performance computing (HPC) project focused on the parallelization of the Mandelbrot set using C and the Message Passing Interface (MPI). This project was developed and benchmarked on the **Setonix supercomputer** at the Pawsey Supercomputing Research Centre.

## Overview

The primary objective of this project was to analyze the performance gains achieved by transitioning from serial algorithms to parallel distributed computing. By comparing static and dynamic workload distribution strategies, this project identifies the most efficient methods for handling the irregular computational density of fractal generation.

## Algorithms Implemented

This project explores three distinct computational strategies:

1. **Serial Baseline:** A single-threaded implementation used to establish a performance benchmark.
2. **Basic Parallel (Static Partitioning):** A parallel implementation using point-to-point communication to distribute fixed segments of the fractal to available nodes.
3. **Dynamic Parallel (Master-Worker Pattern):** An advanced implementation utilizing dynamic load balancing to assign tasks to nodes in real-time, significantly improving efficiency by minimizing node idle time.

## Key Features

* **Parallel Processing:** Implemented using MPI to handle inter-process communication and workload distribution.
* **HPC Optimized:** Configured specifically for the Setonix supercomputer architecture.
* **Performance Analysis:** Includes logic for precise timing and performance metrics to measure speedup and efficiency.
* **Dynamic Scaling:** Designed to handle varying resolutions and iteration depths to stress-test node utilization.

## Tech Stack

* **Language:** C
* **Parallel Framework:** MPI (Message Passing Interface)
* **Environment:** Setonix (Pawsey Supercomputing Research Centre)

## Performance Metrics

The project includes a comprehensive analysis of:
* **Execution Time:** Comparison across all three algorithms using varying core counts.
* **Speedup Factor:** Measuring the ratio of serial execution time to parallel execution time.
* **Efficiency:** Evaluating node utilization, particularly the performance boost provided by dynamic load balancing over static partitioning.

## Getting Started

### Prerequisites

* Access to an HPC environment with an MPI implementation (e.g., OpenMPI or MPICH).
* GCC or a similar C compiler.

### Compilation

You can compile any of the implementations using `mpicc`. For example, to compile the dynamic version:
```bash
mpicc -o mandelbrot_dyn mandelbrot_dyn_timed.c -lm
```

### Execution

To run the program with a specific number of processes (e.g., 16):
```bash
mpirun -np 16 ./mandelbrot_dyn
```

## Project Structure

* **`mandelbrot_serial_timed.c`**: Baseline serial implementation used for performance benchmarking.
* **`mandelbrot_bp_timed.c`**: Basic parallel implementation utilizing point-to-point communication.
* **`mandelbrot_dyn_timed.c`**: Advanced parallel implementation featuring dynamic load balancing.
* **`Performance_Analysis_Parallel_Mandelbrot_Generation.pdf`**: Comprehensive technical report documenting speedup, efficiency, and scalability analysis.
* **`slurm_script.sh`**: Job submission script for the Setonix (Pawsey) Supercomputing cluster.
