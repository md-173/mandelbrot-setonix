#define MAXITER 1000
#define N       8000

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size, chunksize;
    int total_pixels = N * N;
    int i, j, k, l, loop;
    float *x_local, *y;
    float complex z, kappa;
    double t_comp, t_wait, t_comm, t0, t1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    chunksize = total_pixels / size;

    x_local = (float *)malloc(chunksize * sizeof(float));

    y = NULL;
    if (rank == 0)
        y = (float *)malloc(total_pixels * sizeof(float));

    MPI_Barrier(MPI_COMM_WORLD);

    // Computation time
    t0 = MPI_Wtime();

    for (l = 0; l < chunksize; l++) {
        loop = l * size + rank;
        i = loop % N;
        j = loop / N;

        kappa = (4.0f * (i - N / 2)) / N
              + (4.0f * (j - N / 2)) / N * I;
        z = kappa;

        k = 1;
        while ((cabs(z) <= 2) && (k++ < MAXITER))
            z = z * z + kappa;

        x_local[l] = log((float)k) / log((float)MAXITER);
    }

    t1 = MPI_Wtime();
    t_comp = t1 - t0;

    // Waiting Time
    t0 = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    t1 = MPI_Wtime();
    t_wait = t1 - t0;

    // Communication Time
    t0 = MPI_Wtime();
    MPI_Gather(x_local, chunksize, MPI_FLOAT,
               y, chunksize, MPI_FLOAT,
               0, MPI_COMM_WORLD);
    t1 = MPI_Wtime();
    t_comm = t1 - t0;

    printf("Rank %2d : comp = %8.4f s  wait = %8.4f s  comm = %8.4f s\n",
           rank, t_comp, t_wait, t_comm);

    if (rank == 0)
        free(y);
    free(x_local);

    MPI_Finalize();
    return 0;
}
