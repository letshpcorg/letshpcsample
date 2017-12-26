#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define CLK CLOCK_MONOTONIC
struct timespec diff(struct timespec start, struct timespec end){
        struct timespec temp;
        if((end.tv_nsec-start.tv_nsec)<0){
                temp.tv_sec = end.tv_sec-start.tv_sec-1;
                temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
        }
        else{
                temp.tv_sec = end.tv_sec-start.tv_sec;
                temp.tv_nsec = end.tv_nsec-start.tv_nsec;
        }
        return temp;
}

int main(int argc, char* argv[])
{
	struct timespec start_e2e, end_e2e, start_alg, end_alg, e2e, alg;
        /* Should start before anything else */
        clock_gettime(CLK, &start_e2e);

        /* Check if enough command-line arguments are taken in. */
        if(argc < 3) {
                printf( "Usage: %s n p \n", argv[0] );
                return -1;
        }

	const int n = atoi(argv[1]);
	const int p = atoi(argv[2]);

	char *problem_name = "matrix_multiplication";
        char *approach_name = "outermost";

        char outputFileName[50];
	
        sprintf(outputFileName,"output/%s_%s_%s_%s_output.txt",problem_name,approach_name,argv[1],argv[2]);


	int** matA = malloc(n * sizeof(int *));
	int** matB = malloc(n * sizeof(int *));
	int** matC = malloc(n * sizeof(int *));
	int i, j;	
	for(i = 0; i < n; i++)
	{
		matA[i] = malloc(n * sizeof(int));
		matB[i] = malloc(n * sizeof(int));
		matC[i] = malloc(n * sizeof(int));
		for(j = 0; j < n; j++)
		{
			matA[i][j] = 2;
			matB[i][j] = 1; // Initialize
			matC[i][j] = 0;	// Initialize
		}
	}

	int k;
	double start = MPI_Wtime();

        clock_gettime(CLK, &start_alg); /* Start the algo timer */

	for(i = 0; i < n; i++) 
	{
		for(j = 0; j < n; j++) 
		{
			for(k = 0; k < n; k++)
				matC[i][j] += (matA[i][k] * matB[k][j]);
		}
	}
	
	double end = MPI_Wtime();
	clock_gettime(CLK, &end_alg);   /* End the algo timer */

        /* Should end before anything else (printing comes later) */
        clock_gettime(CLK, &end_e2e);
        e2e = diff(start_e2e, end_e2e);
        alg = diff(start_alg, end_alg);

        printf("%s,%s,%d,%d,%d,%ld,%d,%ld\n", problem_name, approach_name, n, p, e2e.tv_sec, e2e.tv_nsec, alg.tv_sec, alg.tv_nsec);;
	free(matA); free(matB); free(matC);
	return 0;
}
