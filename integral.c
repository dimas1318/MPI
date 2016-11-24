#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "mpi.h"

double func(double x)
{
	//return sin(x * x + cos(3 * x)); //need to write -lm
	return 4 / (1 + x * x);
}

int main(int argc, char* argv[])
{

	srand(time(0));

	//double a = -3.0, b = 5.0; //bounds of integration

	int numtasks, rank, res;
	double start, finish, speedup, t_one, t_many;

	res = MPI_Init(&argc, &argv);

	double a = atof(argv[1]), b = atof(argv[2]); //mpirun -np 8 test.out -3.0 5.0
	unsigned long numIters = atoi(argv[4]);

	MPI_Status stat;
	if (res != MPI_SUCCESS)
	{ 
		printf("Initilization failed\n");
		MPI_Abort(MPI_COMM_WORLD, res);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (argc < 5) 
	{ 
		if(!rank)
		{
			printf("A few parameters!\n");
			printf("Please use: mpirun -np (num_procs) test.out (from) (to) (method)\n");
		}
		MPI_Finalize();
		return 0; 
	} 

	double stepForProccess = (b - a) / numtasks; //lenght of integration range for each proccess

	double sum = 0.0, x;
	//unsigned long numIters = 10000000;
	double new_a = a + rank * stepForProccess;	//for each proccess its integration range
	double new_b = a + (rank + 1) * stepForProccess;
	double width_rect = (new_b - new_a) / numIters;

	switch(atoi(argv[3]))
	{
		case 1:
		{
			/* Monte-Carlo method */
			if(!rank)
				start = MPI_Wtime();
			for(int i = 0; i < numIters; i++)
			{
				x = new_a + ((new_b - new_a) * rand() / RAND_MAX);
				sum += func(x);
			} 
			sum *= ((new_b - new_a) / numIters);

			/*if(rank)
				MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
				//MPI_Bcast(&sum, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
			else
			{
				/*for(int i = 1; i < numtasks; i++)
				{
					double new_sum;
					MPI_Recv(&new_sum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &stat);
					sum += new_sum;
				}*/
				/*double new_sum;
				MPI_Reduce(&new_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
				//printf("proc %d calculated %f\n", rank, sum); 
				printf("Monte-Carlo method: %f\n", sum);
			}*/
			double new_sum;
			MPI_Reduce(&sum, &new_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
			if(!rank)
			{
				finish = MPI_Wtime();
				t_many = finish - start;
				//double speedup = t_single / t_paral;
				//double efficiency = speedup / numtasks;
				//printf("speedup - %f, efficiency - %f\n", speedup, efficiency);
				printf("Monte-Carlo method: %f\n", new_sum);
			}
			if(!rank)
			{
				start = MPI_Wtime();
				sum = 0;
				for(int i = 0; i < numIters; i++)
				{
					x = a + ((b - a) * rand() / RAND_MAX);
					sum += func(x);
				} 
				finish = MPI_Wtime();
				printf("Monte-Carlo method: %f\n", sum / numIters);
				t_one = finish - start;
				speedup = t_one / t_many;
				double efficiency = speedup / numtasks;0
				printf("speedup - %f, efficiency - %f\n", speedup, efficiency);
			}
			break;
		}

		case 2:
		{
			/* rectangle method */ 

			for(int i = 0; i < numIters; i++)
			{
				x = new_a + i * width_rect;
				sum += func(x) * width_rect;
			}
			if(rank)
				MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
			else
			{
				for(int i = 1; i < numtasks; i++)
				{
					double new_sum;
					MPI_Recv(&new_sum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &stat);
					sum += new_sum;
				}
				//printf("proc %d calculated %f\n", rank, sum);
				printf("rectangle method: %f\n", sum); 
			}
			break;
		}

		case 3:
		{
			/* trapezoid method */

			for(int i = 0; i < numIters; i++)
			{
				double a = new_a + i * width_rect;
				double b = new_a + (i + 1) * width_rect;
				sum += (func(a) + func(b)) / 2 * width_rect;
			}
			if(rank)
				MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
			else
			{
				for(int i = 1; i < numtasks; i++)
				{
					double new_sum;
					MPI_Recv(&new_sum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &stat);
					sum += new_sum;
				}
				//printf("proc %d calculated %f\n", rank, sum); 
				printf("trapezoid method: %f\n", sum);
			}
			break;
		}
		case 4:
		{
			/* Simpson's rule */

			for(int i = 0; i < numIters; i++)
			{
				double a = new_a + i * width_rect;
				double b = new_a + (i + 1) * width_rect;
				sum += (func(a) + 4 * func((a + b) / 2) + func(b)) * (width_rect / 6);
			}
			if(rank)
				MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
			else
			{
				for(int i = 1; i < numtasks; i++)
				{
					double new_sum;
					MPI_Recv(&new_sum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &stat);
					sum += new_sum;
				}
				//printf("proc %d calculated %f\n", rank, sum); 
				printf("Simpson's rule: %f\n", sum);
			}
			break;
		}
		default:
		{
			if(!rank)
				printf("wrong input method's case\n");
			break;
		}
	}

	MPI_Finalize();
	return 0;
}