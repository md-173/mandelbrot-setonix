
#define MAXITER 1000
#define N	8000

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <sys/time.h>

int main() {
    int	   i,j,k,tenth;
    short  green,blue;
    float  **x;
    FILE   *fp;
    float complex   z, kappa;

    x=(float**)malloc(N*sizeof(float*));
    for (i=0;i<N;i++) {
      x[i]=(float*)malloc(N*sizeof(float));
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            kappa = (4.0*(i-N/2))/N + (4.0*(j-N/2))/N * I;

            z = kappa;

	    k=1;
	    while ((cabs(z)<=2) && (k++<MAXITER)) {
	        z = z*z + kappa;
            }

	    x[i][j]=log((float)k) / log((float)MAXITER);
        }

        tenth=(int) N/10;
        if ((i+1)%tenth==0)
            printf("%d%% complete.\n",100*(i+1)/N);
    }

    gettimeofday(&end, NULL);
    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Calculation time: %f seconds\n", time_taken);

    printf("Writing mandelbrot.ppm\n");
    fp = fopen ("mandelbrot.ppm", "w");
    fprintf (fp, "P6\n%4d %4d\n255\n", N, N);

    for (j=0; j<N; j++)
        for (i=0; i<N; i++)
	    if (x[i][j]<0.5) {
	        green= (int) (2*x[i][j]*255);
                fprintf (fp, "%c%c%c", 255-green,green,0);
	    } else {
	        blue= (int) (2*x[i][j]*255-255);
                fprintf (fp, "%c%c%c", 0,255-blue,blue);
	    }

    fclose(fp);

    for (i=0; i<N; i++)
        free(x[i]);
    free(x);
}
