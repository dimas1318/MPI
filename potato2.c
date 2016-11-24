#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#define POTATOES 10
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

	int lucky_num = rand() % numtasks;

	if(rank == lucky_num)
	{
		potato = rand() % POTATOES;
		MPI_Send(&potato, 1, MPI_INT, (lucky_num + 1) % numtasks, 0, MPI_COMM_WORLD);
		printf("I'm lucky: proc %d sent %d\n", rank, potato);
	}

	int flag = 1;
	while(flag == 1)
	{
		MPI_Recv(&potato, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &stat);

		if (potato == -1)
		{
			flag = 0;
			MPI_Finalize();
		}
		else if(potato == POTATOES)
		{
			potato = -1;
			for(int i = 0; i < numtasks; i++)
			{
				if(i = rank)
					i++;
				MPI_Send(&potato, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			}
			printf("proc %d lost\n", rank);

			//MPI_Bcast(&potato, 1, MPI_INT, 0, MPI_COMM_WORLD);
			flag = 0;
		}
		else
		{
			potato += 1;
			MPI_Send(&potato, 1, MPI_INT, (rank + 1) % numtasks, 0, MPI_COMM_WORLD);
			printf("proc %d sent %d\n", rank, potato);
		}
	}
	printf("proc %d finished\n", rank);
	MPI_Finalize();
	return 0;
}