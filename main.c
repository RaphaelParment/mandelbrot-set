#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <stdbool.h>
#include <strings.h>

// #include "plt/gnuplot_i.h"

#define NRB_ITER 50
#define NUM_COMMANDS 2

double complex f(double complex n, double complex c);
bool belong(double complex c);

int main(int argc, char **argv)
{
    double xmin = -2.0;
    double xmax = 2.0;
    double ymin = -2.0;
    double ymax = 2.0;
    
    double step = 0.01;
    double xrange = xmax - xmin;
    double yrange = ymax - ymin;
    int size = (int)(xrange / step) * (int)(yrange / step);
    printf("size: %d\n", size);

    char * commandsForGnuplot[] = {
        "set title \"Mandelbrot\"", 
        "plot 'data.temp'"};
    double *xvals = (double *)malloc(size * sizeof(double));
    double *yvals = (double *)malloc(size * sizeof(double));
    
    bzero(xvals, size);
    bzero(yvals, size);

    FILE * temp = fopen("data.temp", "w");
    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
    double i, j;
    int k, t = 0;
    // double   complex c;
    for(i = xmin; i < xmax; i+=step)
    {
        for(j = ymin; j < ymax; j+=step)
        {
            double complex c = i + j*I;
            printf("%.2f %+.2fi\n", creal(c), cimag(c));
            if(belong(c))
            {
                xvals[k] = creal(c);
                yvals[k] = cimag(c);
                // printf("%.2f %+.2fi\n", xvals[k],  yvals[k]);
                // printf("%.2f %+.2fi\n", creal(c),  cimag(c));
                k++;
            }
        }
    }
    printf("k: %d\n", k);
    for (t=0; t < k; t++)
    {
        fprintf(temp, "%.3f %.3f \n", xvals[t], yvals[t]);
    }

    for (t=0; t < NUM_COMMANDS; t++)
    {
        fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[t]);
    }

    free(xvals);
    free(yvals);

    return 0;
}

double complex f(double complex n, double complex c) 
{
    return n*n + c;
}

bool belong(double complex c)
{
    short i;
    double complex res = i;
    for (i = 0; i < NRB_ITER; i++)
    {
        res = f(res, c);
        if (cabs(res) > 2.0)
            return false;
    }
    return true;
}