#include <stdio.h>
#include "mpi.h"

#define BUFFSIZE 8
#define TAG 0
#define ITERS 1e7

int main(int argc, char* argv[])
{
	int rank, numtasks, res;
	long i = ITERS;
	char buff[BUFFSIZE];
	MPI_Status stat;
	res = MPI_Init(&argc, &argv);
	if (res != MPI_SUCCESS)
	{
		printf("Initilization failed\n");
		MPI_Abort(MPI_COMM_WORLD, res);
	}
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double time = 0.0;
	if (rank == 0)
		time = MPI_Wtime();

	while(i)
	{
		if (rank == 0)
		{
			MPI_Send(buff, BUFFSIZE, MPI_CHAR, 1, TAG, MPI_COMM_WORLD);
			MPI_Recv(buff, BUFFSIZE, MPI_CHAR, 1, TAG, MPI_COMM_WORLD, &stat);
			i--; 
		}
		if (rank == 1)
		{
			MPI_Recv(buff, BUFFSIZE, MPI_CHAR, 0, TAG, MPI_COMM_WORLD, &stat);
			MPI_Send(buff, BUFFSIZE, MPI_CHAR, 0, TAG, MPI_COMM_WORLD);
			i--; 
		}
	}

	if (rank == 0)
	{
		time = MPI_Wtime() - time;
		printf("time: %.1f s\n", time);
		printf("channel capacity: %f Mb/s\n", ((BUFFSIZE*ITERS)/time)/(1024*1024));
	}

	MPI_Finalize();
	return 0;
}