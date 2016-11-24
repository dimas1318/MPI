#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define n 10000000
int main(int argc, char** argv)
{
	int numtasks, rank, res;

	double tmp;
	double start, finish;
	int* arr = calloc(sizeof(int), n);
	for (int i = 0; i < n; i++)
		arr[i] = 1;

	res = MPI_Init(&argc, &argv);
	MPI_Status stat;
	if (res != MPI_SUCCESS)
	{
		printf("Initilization failed\n");
		MPI_Abort(MPI_COMM_WORLD, res);
	}
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0)
	{
		start = MPI_Wtime();

		int sum = 0;
		for(int i = 1; i < numtasks - 1; i++)
		{
			MPI_Recv(&tmp, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &stat);
			sum += tmp;
		}	

		finish = MPI_Wtime();

		double t_paral = finish - start;

		MPI_Recv(&tmp, 1, MPI_DOUBLE, numtasks - 1, 0, MPI_COMM_WORLD, &stat);
		double t_single = tmp;
		printf("final sum(parallel) %d\n", sum);
		printf("timer(parallel) %f\n", t_paral); 

		double speedup = t_single / t_paral;
		double efficiency = speedup / (numtasks - 2);
		printf("speedup - %f, efficiency - %f\n", speedup, efficiency);
	}
	else if (rank == numtasks - 1)
	{
		start = MPI_Wtime();
		int sum = 0;
		for (int i = 0; i < n; i++)
			sum += arr[i];
		finish = MPI_Wtime();
		tmp = finish - start;
		printf("final sum(single) %d\n", sum);
		printf("timer(single) %f\n", tmp);
		MPI_Send(&tmp, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	else 
	{
		int part = 0;
		if (n % (numtasks - 2))
			part = n / (numtasks - 2) + 1;
		else
			part = n / (numtasks - 2);

		for(int i = 0; i < part; i++)
			tmp += arr[(rank - 1) * part + i];
		printf("proc %d my sum %d \n", rank, (int)tmp);
		MPI_Send(&tmp, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();

	free(arr);
	return 0;
}