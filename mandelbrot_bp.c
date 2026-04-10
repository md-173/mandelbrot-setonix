#define MAXITER 1000
#define N	8000
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int	   i, j, k, loop, rank, size, strip_size, start, end;
    short  green, blue;
    float  *x, *x_all = NULL;
    FILE   *fp;
    float complex   z, kappa;
   
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    strip_size = (N*N) / size;
    start = strip_size * rank;

    x = (float *) malloc(strip_size*sizeof(float));
    if (rank == 0) {
        x_all = (float *) malloc((long)N * N * sizeof(float));
    }
    for (loop = 0; loop < strip_size; loop++) {
            // i:cols and j:rows
            i = (start + loop) % N;
            j = (start + loop) / N;
	        kappa= (4.0*(i-N/2))/N + (4.0*(j-N/2))/N * I;
            z=kappa;
	        k=1;
	        
            while ((cabs(z)<=2) && (k++<MAXITER)) {
	            z= z*z + kappa;
            }
	        x[loop]= log((float)k) / log((float)MAXITER);
    }
    
    MPI_Gather(x, strip_size, MPI_FLOAT, x_all, strip_size, MPI_FLOAT, 0, MPI_COMM_WORLD);
    if (rank == 0) {  
        printf("Writing mandelbrot2.ppm\n");
        fp = fopen ("mandelbrot2.ppm", "w");
        fprintf (fp, "P6\n%4d %4d\n255\n", N, N);
    
        for (loop=0; loop<N*N; loop++) 
	        if (x_all[loop]<0.5) {
	            green= (int) (2*x_all[loop]*255);
                    fprintf (fp, "%c%c%c", 255-green,green,0);
	        } else {
	            blue= (int) (2*x_all[loop]*255-255);
                    fprintf (fp, "%c%c%c", 0,255-blue,blue);
	        }
        fclose(fp);
        free(x_all);
    }
    free(x);
    MPI_Finalize();
    return 0;
}
