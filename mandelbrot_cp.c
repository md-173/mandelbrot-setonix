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
    int i, j, k, l, r, loop;
    short green, blue;
    float *x_local, *x, *y;
    float complex z, kappa;
    FILE *fp;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    chunksize = total_pixels / size;

    x_local = (float *)malloc(chunksize * sizeof(float));

    // Compute
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

    // Gather the data
    y = NULL;
    x = NULL;
    if (rank == 0) {
        y = (float *)malloc(total_pixels * sizeof(float));
        x = (float *)malloc(total_pixels * sizeof(float));
    }

    MPI_Gather(x_local, chunksize, MPI_FLOAT,
               y, chunksize, MPI_FLOAT,
               0, MPI_COMM_WORLD);

    // Reorder on rank 0
    if (rank == 0) {
        for (r = 0; r < size; r++) {
            for (l = 0; l < chunksize; l++) {
                loop = l * size + r;
                x[loop] = y[r * chunksize + l];
            }
        }

        printf("Writing mandelbrot_cyclic.ppm\n");
        fp = fopen("mandelbrot_cyclic.ppm", "w");
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
        free(y);
    }

    free(x_local);
    MPI_Finalize();
    return 0;
}
