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

	char *problem_name = "matrix_multiplication";
        char *approach_name = "outermost";

        char outputFileName[50];
        sprintf(outputFileName,"output/%s_%s_%s_%s_output.txt",problem_name,approach_name,argv[1],argv[2]);

	MPI_Init(NULL, NULL);

	const int n = atoi(argv[1]);
	const int p = atoi(argv[2]);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank)
;
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
			matB[i][j] = 1; // Initialize
			matC[i][j] = 0;	// Initialize
		}	
	}	

	// Total number of processors (colleagues :P)
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	if(world_rank == 0)
	{
		for(i = 0; i < n; i++)
		{
			for(j = 0; j < n; j++)
				matA[i][j] = 2;
		}
		int destination;
		double start = MPI_Wtime();

		clock_gettime(CLK, &start_alg); /* Start the algo timer */

		for(destination = 1; destination < world_size; destination++) 
		{	
			int start = destination * (n / world_size);
			int end = (destination + 1) * (n / world_size);
			if(destination == world_size - 1)
				end = n;
			int offset = start;
			int rows = (end - start);
			MPI_Send(&offset, 1, MPI_INT, destination, 1, MPI_COMM_WORLD); // Send number of rows
			MPI_Send(&rows, 1, MPI_INT, destination, 2, MPI_COMM_WORLD); // Send number of rows
			MPI_Send(&matA[offset][0], rows * n, MPI_INT, destination, 3, MPI_COMM_WORLD); // Send portion of matrix A	
		}
		double sending = MPI_Wtime();

		// Do matrix multiplication specific to master processor
		int k;
		int rows = n / world_size;
		for(i = 0; i < rows; i++) 
		{
			for(j = 0; j < n; j++) 
			{
				for(k = 0; k < n; k++)
					matC[i][j] += (matA[i][k] * matB[k][j]);
			}
		}

		// Wait for other processors to complete and combine their results
		double receiving = MPI_Wtime();
		int source;
		for(source = 1; source < world_size; source++) 
		{		
			int offset, rows;
			MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receive offset
			MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receive number of rows
			MPI_Recv(&matC[offset][0], rows * n, MPI_INT, source, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receive portion of matrix C
		}
		double end = MPI_Wtime();
		clock_gettime(CLK, &end_alg); /* End the algo timer */
		// free(matA); free(matB); free(matC);
		clock_gettime(CLK, &end_e2e);
        	e2e = diff(start_e2e, end_e2e);
	        alg = diff(start_alg, end_alg);
		printf("%s,%s,%d,%d,%d,%ld,%d,%ld\n", problem_name, approach_name, n, p, e2e.tv_sec, e2e.tv_nsec, alg.tv_sec, alg.tv_nsec);
	}
	else 
	{
		int offset;
		int rows;
		MPI_Recv(&offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);     // Receive offset
		MPI_Recv(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 	    // Receive number of rows
		MPI_Recv(&matA[offset][0], rows * n, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Receive portion of matrix A

		int k;

		// Do matrix multiplication
		for(i = offset; i < offset + rows; i++) {
			for(j = 0; j < n; j++) {
				for(k = 0; k < n; k++) {
					matC[i][j] += (matA[i][k] * matB[k][j]);
				}
			}
		}

		// Send answers to master processor
		MPI_Send(&offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD); // Send offset
		MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD); // Send number of rows
		MPI_Send(&matC[offset][0], rows * n, MPI_INT, 0, 3, MPI_COMM_WORLD); // Send portion of matrix C
	}
	free(matA); free(matB); free(matC);
	MPI_Finalize();
}
