#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

double f(double t, double x){
    return sin(t) + cos(x);
}

double fi(double x){
	return sin(x);
}

double ksi(double t){
    return sin(2*t);
}

int main(int argc, char *argv[]){
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

	double h = 0.1;
    double t = 0.3;
    double U[numtasks + 1][numtasks + 1];
    double node[numtasks];
    double tmp;

    for(int i = 0; i <= numtasks; i++)
    	U[0][i] = fi(i * h);
    for(int i = 0; i <= numtasks; i++)
    	U[i][0] = ksi(i * t);

    U[1][rank + 1] = (f(t, (rank + 1) * h) - (U[0][rank + 1] - U[0][rank])/h)*t + U[0][rank + 1];
    node[0] = U[1][rank + 1];
    if(rank != numtasks - 1){
	   	//printf("proc %d sends %f\n", rank, tmp);
	   	MPI_Send(&node[0], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
	}

    if(!rank){
    	for(int i = 1; i < numtasks; i++){
			U[i + 1][rank + 1] = (f(i * t, (rank + 1) * h) - (U[i][rank + 1] - U[i][rank])/h)*t + U[i][rank + 1];
			tmp = U[i + 1][rank + 1];
			//printf("proc %d sends %f\n", rank, tmp);
			MPI_Send(&tmp, 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
		}
    }
    else{
    	for(int i = 1; i < numtasks; i++){
	    	MPI_Recv(&tmp, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &stat);
	    	U[i + 1][rank + 1] = (f(i * t, (rank + 1) * h) - (U[i][rank + 1] - tmp)/h)*t + U[i][rank + 1];
	    	node[i] = U[i + 1][rank + 1];
	    	tmp = node[i];
	    	if(rank != numtasks - 1){
	    		//printf("proc %d sends %f\n", rank, tmp);
	    		MPI_Send(&tmp, 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
	    	}
	    }
    }
    //MPI_Barrier(MPI_COMM_WORLD);

    /*for(int i = 0; i < numtasks; i++)
			printf("%f ", node[i]);
	printf("\n");
	printf("\n");*/

    if(rank)
		MPI_Send(&node, numtasks, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	else{
		for(int i = 1; i < numtasks; i++){
			MPI_Recv(&node, numtasks, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &stat);
			//printf("sent from %d\n", i);
			for(int j = 1; j <= numtasks; j++)
				U[j][i + 1] = node[j - 1];
		}

		for(int i = 0; i <= numtasks; i++){
			for(int j = 0; j <= numtasks; j++)
				printf("%f ", U[i][j]);
			printf("\n");
		}
	}

	MPI_Finalize();
	return 0;
}