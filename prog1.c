#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv)
{
	int res, rank, num_task;
	res = MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_task);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("razberi elky %d \n", rank);

	MPI_Finalize();
	return 0;
}
/* 
	mpicc prog1.c -o test.out
	mpirun -np 4 test.out
*/