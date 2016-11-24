#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define SIZE 1000000
int main(int argc, char** argv)
{
        char* mas = malloc(SIZE);
        memset(mas, 1, SIZE);
        int i;
        int rr = 0;
        double time1 = MPI_Wtime();
        for (i = 0; i < SIZE; i++)
        {
                rr += mas[i];
        }
        time1 = MPI_Wtime() - time1;
        MPI_Init(&argc,&argv);
        double start_time = MPI_Wtime();
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        int size;
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        int result = 0;
        int size_of_one_thread;
        if (size == 1) size_of_one_thread = SIZE;
        else size_of_one_thread = SIZE / (size - 1);
        for (i = rank * size_of_one_thread; (i < (rank + 1) * size_of_one_thread) && (i < SIZE); i++)
                result += mas[i];
        if (rank == size - 1)
        {
                int r = result;
                for (i = 0; i < rank; i++)
                {
                        MPI_Status status;
                        int sub_res;
                        MPI_Recv(&sub_res, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                        result += sub_res;
//                      printf("result[%d] = %d\n", i, sub_res);
                }
//              printf("result[%d] = %d\n", rank, r);
                double end_time = MPI_Wtime();
                printf("End result is %d\nElapsed time(parallel): %f sec\n", result, end_time - start_time);
                printf("Elapsed time(non parallel): %f\n", time1);
                printf("Number of processors: %d\n", size);
                double S = time1/(end_time - start_time);
                printf("Speedup:    %f\nEfficiency: %f\n", S, S/size);
        }
        if (rank != size - 1)
        {
                MPI_Send(&result, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD);
        }
        free(mas);
        MPI_Finalize();
        return 0;
}
