#define MAXITER 1000
#define N       8000

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int    i, j, k;
    float  *x_local, *x_global = NULL;
    int    rank, size;
    int    rows_per_proc, local_size;
    int    j_start;
    double t_comp, t_wait, t_comm;
    double t0, t1;

    float complex z, kappa;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    rows_per_proc = N / size;
    local_size    = rows_per_proc * N;
    j_start       = rank * rows_per_proc;

    x_local = (float *)malloc(local_size * sizeof(float));

    if (rank == 0)
        x_global = (float *)malloc((long)N * N * sizeof(float));

    // Sync process before timer
    MPI_Barrier(MPI_COMM_WORLD);

    // Start computation
    t0 = MPI_Wtime();

    for (int loc = 0; loc < local_size; loc++) {
        i = loc % N;
        j = j_start + loc / N;

        kappa = (4.0f * (i - N / 2)) / N
              + (4.0f * (j - N / 2)) / N * I;
        z = kappa;

        k = 1;
        while ((cabs(z) <= 2) && (k++ < MAXITER))
            z = z * z + kappa;

        x_local[loc] = log((float)k) / log((float)MAXITER);
    }

    t1 = MPI_Wtime();
    t_comp = t1 - t0;

    // Waiting Start
    t0 = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    t1 = MPI_Wtime();
    t_wait = t1 - t0;

    // Communication start
    t0 = MPI_Wtime();
    MPI_Gather(x_local,  local_size, MPI_FLOAT,
               x_global, local_size, MPI_FLOAT,
               0, MPI_COMM_WORLD);
    t1 = MPI_Wtime();
    t_comm = t1 - t0;

    printf("Rank %2d : comp = %8.4f s  wait = %8.4f s  comm = %8.4f s\n",
           rank, t_comp, t_wait, t_comm);

    /*
     * PPM output commented out
     */

    if (rank == 0)
        free(x_global);
    free(x_local);

    MPI_Finalize();
    return 0;
}
