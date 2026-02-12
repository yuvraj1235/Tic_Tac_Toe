#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <time.h>
#include "myHeader.h"

/* replace column for Cramer's rule */
int **replaceColumn(int **A,int **B,int n,int col)
{
    int **temp = alloc2DArr(n,n);
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            temp[i][j] = (j==col) ? B[i][0] : A[i][j];
    return temp;
}

int main()
{
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);   // ⏱ start time

    int n;
    scanf("%d",&n);

    int **A = alloc2DArr(n,n);
    int **B = alloc2DArr(n,1);

    read2DArr(A,n,n);
    read2DArr(B,n,1);

    /* ===== parent computes D ===== */
    int D = matrixDet(A,n);

    FILE *fp = fopen("output.txt","w");
    if(fp == NULL) exit(1);

    if(D == 0)
    {
        fprintf(fp,"No unique solution (D = 0)\n");
        fclose(fp);
        return 0;
    }

    fprintf(fp,"D = %d\n",D);
    fclose(fp);

    /* ===== children compute Dx ===== */
    for(int i=0;i<n;i++)
    {
        pid_t pid = fork();

        if(pid == 0)
        {
            int **Ai = replaceColumn(A,B,n,i);
            int Di = matrixDet(Ai,n);
            double xi = (double)Di / D;

            FILE *cfp = fopen("output.txt","a");
            if(cfp)
            {
                fprintf(cfp,"D%d = %d, x%d = %.4lf\n",
                        i+1, Di, i+1, xi);
                fclose(cfp);
            }
            exit(0);
        }
    }

    for(int i=0;i<n;i++)
        wait(NULL);

    clock_gettime(CLOCK_MONOTONIC, &end);   // ⏱ end time

    double time_taken =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1e9;

    fp = fopen("output.txt","a");
    if(fp)
    {
        fprintf(fp,"Time taken = %.6f seconds\n", time_taken);
        fclose(fp);
    }

    return 0;
}
