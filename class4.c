#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define n 10000000
int main(int argc, char** argv)
{
	int numtasks, rank, res;

	int tmp = 0;
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

	if (!rank)
	{
		int sum = 0;
		for(int i = 1; i < numtasks; i++)
		{
			MPI_Recv(&tmp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &stat);
			sum += tmp;
		}
		printf("final sum %d\n", sum);
	}
	else
	{
		int part = 0;
		if (n % (numtasks - 1))
			part = n / (numtasks - 1) + 1;
		else
			part = n / (numtasks - 1);
		for(int i = 0; i < part; i++)
			tmp += arr[(rank - 1) * part + i];
		printf("proc %d my sum %d \n", rank, tmp);
		MPI_Send(&tmp, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();

	free(arr);
	return 0;
}