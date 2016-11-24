#include <stdio.h>
#include "mpi.h"
#include <math.h>

#define n 100
int main(int argc, char** argv)
{
	int numtasks, rank, res;
	double buff[n], start, finish;
	for (int i = 0; i < n; i++)
		buff[i] = 0.0;
	res = MPI_Init(&argc, &argv);
	MPI_Status stat;
	if (res != MPI_SUCCESS)
	{
		printf("Initilization failed\n");
		MPI_Abort(MPI_COMM_WORLD, res);
	}
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	

	if (rank != 0)
	{
		MPI_Recv(&buff, n, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &stat);
		finish = MPI_Wtime ();
		buff[2*rank - 1] = finish;
		buff[rank - 1] = finish - buff[2*rank - 2];

		printf("Process %d completed %f\n", rank, buff[rank - 1]);

		if (rank == numtasks - 1)
		{
			printf("I'm here\n");
			double sum = 0.0;
			for (int i = 0; i < rank; i++)
				sum += buff[i];
			printf("%f\n", sum / rank);
		}
	}

	if (rank != numtasks - 1)
	{
		start = MPI_Wtime ();
		buff[rank * 2] = start;
		MPI_Send(&buff, n, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
		if (rank == 0)
			printf("Process %d completed\n", rank);
	}

	MPI_Finalize();
	return 0;
}