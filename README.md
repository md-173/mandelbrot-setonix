# Parallel Mandelbrot Generation on Setonix

A high-performance computing (HPC) project focused on the parallelization of the Mandelbrot set using C and the Message Passing Interface (MPI). This project was developed and benchmarked on the **Setonix supercomputer** at the Pawsey Supercomputing Research Centre.

## Overview

The primary objective of this project was to analyze the performance gains achieved by transitioning from serial algorithms to parallel distributed computing. By utilizing MPI, the workload is distributed across multiple compute nodes, significantly reducing the computational time required for high-resolution fractal generation.

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
* **Execution Time:** Comparison between serial and parallel execution across various core counts.
* **Speedup Factor:** Measuring the ratio of serial execution time to parallel execution time.
* **Efficiency:** Evaluating how effectively hardware resources are utilized as node counts increase.

## Getting Started

### Prerequisites

* Access to an HPC environment with an MPI implementation (e.g., OpenMPI or MPICH).
* GCC or a similar C compiler.

### Compilation

Use the following command to compile the program:
```bash
mpicc -o mandelbrot mandelbrot.c -lm
```

### Execution

To run the program with a specific number of processes (e.g., 4):
```bash
mpirun -np 4 ./mandelbrot
```

## Project Structure

* `mandelbrot.c`: Core implementation containing Mandelbrot logic and MPI orchestration.
* `slurm_script.sh`: Example Slurm script for job submission on Setonix.
* `results/`: Directory containing timing data and performance visualizations.
