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
    int next_start, finished_workers, recv_index, src;
    int start, idx, i, j, k, loop, w;
    short green, blue;
    float *x, *recv_buf, *buf;
    float complex z, kappa;
    FILE *fp;
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

    // Master
    if (rank == 0) {
        x = (float *)malloc(total_pixels * sizeof(float));
        recv_buf = (float *)malloc(chunksize * sizeof(float));
        next_start = 0;
        finished_workers = 0;

        for (w = 1; w <= num_workers; w++) {
            if (next_start < total_pixels) {
                MPI_Send(&next_start, 1, MPI_INT, w, 0, MPI_COMM_WORLD);
                next_start += chunksize;
            } else {
                MPI_Send(&total_pixels, 1, MPI_INT, w, 0, MPI_COMM_WORLD);
                finished_workers++;
            }
        }

        while (finished_workers < num_workers) {
            MPI_Recv(&recv_index, 1, MPI_INT, MPI_ANY_SOURCE, 0,
                     MPI_COMM_WORLD, &status);
            src = status.MPI_SOURCE;

            MPI_Recv(recv_buf, chunksize, MPI_FLOAT, src, 1,
                     MPI_COMM_WORLD, &status);

            for (loop = 0; loop < chunksize && (recv_index + loop) < total_pixels; loop++)
                x[recv_index + loop] = recv_buf[loop];

            if (next_start < total_pixels) {
                MPI_Send(&next_start, 1, MPI_INT, src, 0, MPI_COMM_WORLD);
                next_start += chunksize;
            } else {
                MPI_Send(&total_pixels, 1, MPI_INT, src, 0, MPI_COMM_WORLD);
                finished_workers++;
            }
        }

        printf("Writing mandelbrot_dyn.ppm\n");
        fp = fopen("mandelbrot_dyn.ppm", "w");
        fprintf(fp, "P6\n%4d %4d\n255\n", N, N);
        for (loop = 0; loop < total_pixels; loop++) {
            if (x[loop] < 0.5f) {
                green = (int)(2 * x[loop] * 255);
                fprintf(fp, "%c%c%c", 255 - green, green, 0);
            } else {
                blue = (int)(2 * x[loop] * 255 - 255);
                fprintf(fp, "%c%c%c", 0, 255 - blue, blue);
            }
        }
        fclose(fp);

        free(x);
        free(recv_buf);

    // Worker
    } else {
        buf = (float *)malloc(chunksize * sizeof(float));

        MPI_Recv(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        while (start < total_pixels) {
            for (loop = 0; loop < chunksize; loop++) {
                idx = start + loop;
                if (idx < total_pixels) {
                    i = idx % N;
                    j = idx / N;
                    kappa = (4.0f * (i - N / 2)) / N
                          + (4.0f * (j - N / 2)) / N * I;
                    z = kappa;
                    k = 1;
                    while ((cabs(z) <= 2) && (k++ < MAXITER))
                        z = z * z + kappa;
                    buf[loop] = log((float)k) / log((float)MAXITER);
                } else {
                    buf[loop] = 0.0f;
                }
            }

            MPI_Send(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Send(buf, chunksize, MPI_FLOAT, 0, 1, MPI_COMM_WORLD);

            MPI_Recv(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        }

        free(buf);
    }

    MPI_Finalize();
    return 0;
}