#define MAXITER 1000
#define N       8000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size, chunksize;
    int total_pixels = N * N;
    int num_workers;
    int next_start, workers_done, recv_index, src;
    int start, idx, i, j, k, loc, w;
    float *x, *recv_buf, *buf;
    float complex z, kappa;
    double t_comp, t_comm, t_total, t0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0) printf("Usage: %s <chunksize>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    chunksize = atoi(argv[1]);
    num_workers = size - 1;

    MPI_Barrier(MPI_COMM_WORLD);
    t_total = MPI_Wtime();

    // WORKER
    if (rank == 0) {
        x = (float *)malloc(total_pixels * sizeof(float));
        recv_buf = (float *)malloc(chunksize * sizeof(float));
        next_start = 0;
        workers_done = 0;

        for (w = 1; w <= num_workers; w++) {
            if (next_start < total_pixels) {
                MPI_Send(&next_start, 1, MPI_INT, w, 0, MPI_COMM_WORLD);
                next_start += chunksize;
            } else {
            MPI_Send(&total_pixels, 1, MPI_INT, w, 0, MPI_COMM_WORLD);
            workers_done++;
            }
        }

        while (workers_done < num_workers) {
            MPI_Recv(&recv_index, 1, MPI_INT, MPI_ANY_SOURCE, 0,
                     MPI_COMM_WORLD, &status);
            src = status.MPI_SOURCE;

            MPI_Recv(recv_buf, chunksize, MPI_FLOAT, src, 1,
                     MPI_COMM_WORLD, &status);

            for (loc = 0; loc < chunksize && (recv_index + loc) < total_pixels; loc++)
                x[recv_index + loc] = recv_buf[loc];

            if (next_start < total_pixels) {
                MPI_Send(&next_start, 1, MPI_INT, src, 0, MPI_COMM_WORLD);
                next_start += chunksize;
            } else {
                MPI_Send(&total_pixels, 1, MPI_INT, src, 0, MPI_COMM_WORLD);
                workers_done++;
            }
        }

        t_total = MPI_Wtime() - t_total;
        printf("Master   : total = %8.4f s\n", t_total);

        free(x);
        free(recv_buf);

    // WORKER
    } else {
        buf = (float *)malloc(chunksize * sizeof(float));
        t_comp = 0.0;
        t_comm = 0.0;

        t0 = MPI_Wtime();
        MPI_Recv(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        t_comm += MPI_Wtime() - t0;

        while (start < total_pixels) {
            t0 = MPI_Wtime();
            for (loc = 0; loc < chunksize; loc++) {
                idx = start + loc;
                if (idx >= total_pixels) {
                    buf[loc] = 0.0f;
                    continue;
                }
                i = idx % N;
                j = idx / N;
                kappa = (4.0f * (i - N / 2)) / N
                      + (4.0f * (j - N / 2)) / N * I;
                z = kappa;
                k = 1;
                while ((cabs(z) <= 2) && (k++ < MAXITER))
                    z = z * z + kappa;
                buf[loc] = log((float)k) / log((float)MAXITER);
            }
            t_comp += MPI_Wtime() - t0;

            t0 = MPI_Wtime();
            MPI_Send(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Send(buf, chunksize, MPI_FLOAT, 0, 1, MPI_COMM_WORLD);
            MPI_Recv(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            t_comm += MPI_Wtime() - t0;
        }

        printf("Rank %2d  : comp = %8.4f s  comm = %8.4f s\n",
               rank, t_comp, t_comm);

        free(buf);
    }

    MPI_Finalize();
    return 0;
}
