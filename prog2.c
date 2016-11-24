#include <stdio.h>
#include "mpi.h"

int main(int argc, char** argv)
{
	int numtasks, rank, res;
	res = MPI_Init(&argc, &argv);
	MPI_Status stat;
	if (res != MPI_SUCCESS)
	{
		printf("Initilization failed\n");
		MPI_Abort(MPI_COMM_WORLD, res);
	}
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//MPI_Barrier(MPI_COMM_WORLD);

	if (rank != 0)
	{
		MPI_Recv(NULL, 0, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &stat);
		printf("Process %d completed\n", rank);
		//fflush(stdout);
	}

	if (rank != numtasks - 1)
	{
		MPI_Send(NULL, 0, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		if (rank == 0)
			printf("Process %d completed\n", rank);
	}

	//printf("Process %d completed\n", rank);

	MPI_Finalize();
	return 0;
}