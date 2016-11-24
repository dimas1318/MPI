#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#define POTATOES 20
#define TAG 0

int main(int argc, char** argv)
{
	srand(time(0));
	int numtasks, rank, res;
	int potato;
	res = MPI_Init(&argc, &argv);
	MPI_Status stat;
	if (res != MPI_SUCCESS)
	{
		printf("Initilization failed\n");
		MPI_Abort(MPI_COMM_WORLD, res);
	}
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int lucky_num = (rand() % (numtasks - 1)) + 1;

	if (!rank)
	{
		potato = rand() % POTATOES;
		for(int i = lucky_num; potato < POTATOES; i++)
		{
			MPI_Recv(NULL, 0, MPI_INT, (i % (numtasks-1)) + 1, 0, MPI_COMM_WORLD, &stat);
			potato += 1;
			if(potato == POTATOES)
				printf("Process %d lost\n", (i % (numtasks-1)) + 1);
			else
				printf("Process %d sent potato %d\n", (i % (numtasks-1)) + 1, potato);
		}
	}
	else
	{
		int c = (POTATOES / numtasks) + 6;
		while(c)
		{
			MPI_Send(NULL, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
			c--;
		}
	}

	MPI_Finalize();
	return 0;
}