#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

//#define n 10000000

void QSort(int* arr, int lb, int rb)
{
	int left = lb;
	int right = rb;
	int elem = arr[(rb + lb) / 2];

	while (left <= right)
	{
		while (arr[left] < elem)
			left++;
		while (arr[right] > elem)
			right--;
		if (left <= right)
		{
			int temp = arr[left];
			arr[left] = arr[right];
			arr[right] = temp;
			left++;
			right--;
		}
	}
	if (lb < right)
		QSort(arr, lb, right);
	if (left < rb)
		QSort(arr, left, rb);
}

void MSort(int* arr, int* buf, int lb, int rb)
{
	int split;
	if(lb < rb)
	{
		split = (lb + rb) / 2;

		MSort(arr, buf, lb, split);
		MSort(arr, buf, split + 1, rb);

		int pos1 = lb; //position of reading from array 1
		int pos2 = split + 1; //position of reading from array 2

		/*merger of 2 arrays, while one of them won't be finished*/
		for (int i = lb; i <= rb; i++)
		{
			if((pos1 <= split) && (pos2 <= rb))
			{
				if (arr[pos1] > arr[pos2])
					buf[i] = arr[pos2++];
				else
					buf[i] = arr[pos1++];
			}

			/*when one of them was finished*/
			else if (pos1 <= split)
				buf[i] = arr[pos1++];
			else
				buf[i] = arr[pos2++];
		}
		for(int i = lb; i <= rb; i++) 	
			arr[i] = buf[i];
	}
}

int main(int argc, char** argv)
{
	srand(time(NULL));

	int numtasks, rank, res;
	double start, finish;
	int n = atoi(argv[1]);

	/*create array for sorting*/
	int* arr = calloc(sizeof(int), n);
	for (int i = 0; i < n; i++)
		arr[i] = n -i;
		//arr[i] = rand() % n;

	/**Initialization MPI*/
	res = MPI_Init(&argc, &argv);
	if (res != MPI_SUCCESS)
	{
		printf("Initilization failed\n");
		MPI_Abort(MPI_COMM_WORLD, res);
	}
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	/*print unsorted array*/
	if(!rank)
	{
		start = MPI_Wtime();
		/*printf("This is unsorted array:\n");
		for (int i = 0; i < n; i++)
			printf("%d ", arr[i]);
		printf("\n");*/
	}

	int size = n / numtasks; //size of each subarray

	/*send subarray to each proccess*/
	int* sub_arr = calloc(sizeof(int), size);
	MPI_Scatter(arr, size, MPI_INT, sub_arr, size, MPI_INT, 0, MPI_COMM_WORLD);

	/*perform QSort to each subarray*/
	QSort(sub_arr, 0, size - 1);

	/*merger of all subarrays into one*/
	int *sorted = NULL;
	if(!rank) 
		sorted = calloc(sizeof(int), n);
	MPI_Gather(sub_arr, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);

	if(!rank) 
	{
		
		/*perform MSort to resulted array*/
		int *other_arr = calloc(sizeof(int), n);
		MSort(sorted, other_arr, 0, n - 1);
		
		/*print sorted array*/
		/*printf("This is sorted array:\n");
		for(int i = 0; i < n; i++) 	
			printf("%d ", sorted[i]);		
		printf("\n");*/

		finish = MPI_Wtime();

		/*check work*/
		for(int i = 0; i < n - 1; i++) 	
		{
			if(sorted[i] > sorted[i + 1])
			{
				printf("unsorted!!!\n");
				break;
			}
			if(i == n - 2)
				printf("it's alright!!!\n");
		}
		printf("time: %fs\n", finish - start);
			
		free(sorted);
		free(other_arr);	
	}

	free(arr);
	free(sub_arr);

	MPI_Finalize();
	return 0;
}