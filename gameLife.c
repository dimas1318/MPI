#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*#define WIDTH 10
#define LENGTH 10*/
#define WIDTH 100
#define LENGTH 60

#define DEAD 0
#define ALIVE 1

void create_world(int arr[LENGTH][WIDTH]){
	for (int i = 0; i < LENGTH; ++i){
		for (int j = 0; j < WIDTH; ++j){
			arr[i][j] = DEAD;
		}
	}
	char c = getchar();
	switch(c - '0'){
	case 1:	/*Bunnies 10*/
		arr[31][32] = ALIVE;
		arr[32][31] = ALIVE;
		arr[32][32] = ALIVE;
		arr[34][31] = ALIVE;
		arr[35][31] = ALIVE;
		arr[36][31] = ALIVE;
		arr[32][34] = ALIVE;
		arr[34][34] = ALIVE;
		arr[33][35] = ALIVE;
		arr[33][36] = ALIVE;
		break;
	case 2:	/*Gosper's Glider Gun*/
		arr[1][25] = ALIVE;
		arr[2][23] = ALIVE;
		arr[2][25] = ALIVE;
		arr[3][13] = ALIVE;
		arr[3][14] = ALIVE;
		arr[3][21] = ALIVE;
		arr[3][22] = ALIVE;
		arr[3][35] = ALIVE;
		arr[3][36] = ALIVE;
		arr[4][12] = ALIVE;
		arr[4][16] = ALIVE;
		arr[4][21] = ALIVE;
		arr[4][22] = ALIVE;
		arr[4][35] = ALIVE;
		arr[4][36] = ALIVE;
		arr[5][1] = ALIVE;
		arr[5][2] = ALIVE;
		arr[5][11] = ALIVE;
		arr[5][17] = ALIVE;
		arr[5][21] = ALIVE;
		arr[5][22] = ALIVE;
		arr[6][1] = ALIVE;
		arr[6][2] = ALIVE;
		arr[6][11] = ALIVE;
		arr[6][15] = ALIVE;
		arr[6][17] = ALIVE;
		arr[6][18] = ALIVE;
		arr[6][23] = ALIVE;
		arr[6][25] = ALIVE;
		arr[7][11] = ALIVE;
		arr[7][17] = ALIVE;
		arr[7][25] = ALIVE;
		arr[8][12] = ALIVE;
		arr[8][16] = ALIVE;
		arr[9][13] = ALIVE;
		arr[9][14] = ALIVE;
		arr[10][24] = ALIVE;
		arr[11][25] = ALIVE;
		arr[11][26] = ALIVE;
		arr[12][24] = ALIVE;
		arr[12][25] = ALIVE;
		arr[18][31] = ALIVE;
		arr[18][33] = ALIVE;
		arr[19][32] = ALIVE;
		arr[19][33] = ALIVE;
		arr[20][32] = ALIVE;
		break;
	case 3:	/*DieHard*/
		arr[15][11] = ALIVE;
		arr[15][12] = ALIVE;
		arr[16][12] = ALIVE;
		arr[14][17] = ALIVE;
		arr[16][16] = ALIVE;
		arr[16][17] = ALIVE;
		arr[16][18] = ALIVE;
		break;
	case 4: /*Iwona*/
		arr[50][21] = ALIVE;
		arr[50][22] = ALIVE;
		arr[51][22] = ALIVE;
		arr[27][23] = ALIVE;
		arr[28][24] = ALIVE;
		arr[28][25] = ALIVE;
		arr[29][24] = ALIVE;
		arr[34][28] = ALIVE;
		arr[34][29] = ALIVE;
		arr[35][29] = ALIVE;
		arr[21][35] = ALIVE;
		arr[21][36] = ALIVE;
		arr[21][37] = ALIVE;
		arr[29][40] = ALIVE;
		arr[30][40] = ALIVE;
		arr[31][40] = ALIVE;
		arr[32][41] = ALIVE;
		arr[33][41] = ALIVE;
		arr[33][40] = ALIVE;
		break;
	default: /*glinder - планер*/
		arr[0][0] = ALIVE;
		arr[1][1] = ALIVE;
		arr[1][2] = ALIVE;
		arr[2][0] = ALIVE;
		arr[2][1] = ALIVE;
		break;
	}
}

void print_world(int arr[LENGTH][WIDTH]){
	system("clear");
	for (int i = 0; i < LENGTH; ++i){
		for (int j = 0; j < WIDTH; ++j){
			if(arr[i][j] == DEAD)
				printf(" ");
			else
				printf("*");
			printf(" ");
		}
		printf("\n");
	}
}

void copy_world(int arr[LENGTH][WIDTH], int arr_new[LENGTH][WIDTH]){
	for (int i = 0; i < LENGTH; ++i){
		for (int j = 0; j < WIDTH; ++j){
			arr_new[i][j] = arr[i][j];
		}
	}
}

int is_anybody_alive(int arr[LENGTH][WIDTH]){
	for (int i = 0; i < LENGTH; ++i){
		for (int j = 0; j < WIDTH; ++j){
			if(arr[i][j] == ALIVE)
				return 1;
		}
	}
	return 0;
}

int is_not_equal(int arr_prev[LENGTH][WIDTH], int arr_cur[LENGTH][WIDTH]){
	for (int i = 0; i < LENGTH; ++i){
		for (int j = 0; j < WIDTH; ++j){
			if(arr_prev[i][j] != arr_cur[i][j])
				return 1;
		}
	}
	return 0;
}

int number_neighbors(int arr[LENGTH][WIDTH], int x, int y){
	int count = 0;
	for(int i = y - 1; i <= y + 1; ++i){
		for (int j = x - 1; j <= x + 1; ++j){
			if(i == y && j == x)
				continue;
			if(i < 0 || j < 0)
				continue;
			if(i >= LENGTH || j >= WIDTH)
				continue;
			if(arr[i][j] == ALIVE)
				++count;
		}
	}
	return count;
}

void change_world(int arr[LENGTH][WIDTH], int arr_new[LENGTH][WIDTH]){
	for (int i = 0; i < LENGTH; ++i){
		for (int j = 0; j < WIDTH; ++j){
			int num_neib = number_neighbors(arr, j, i);
			if(arr[i][j] == DEAD){
				if(num_neib == 3)
					arr_new[i][j] = ALIVE;
				else
					arr_new[i][j] = DEAD;
			}
			else{
				if(num_neib == 2 || num_neib == 3)
					arr_new[i][j] = ALIVE;
				else
					arr_new[i][j] = DEAD;	
			}
		}
	}
}

int main(int argc, char const *argv[]){
	int world[LENGTH][WIDTH];
	int prev_world[LENGTH][WIDTH];
	create_world(world);

	do{
		print_world(world);
		copy_world(world, prev_world);
		//sleep(1000);
		/*make new world*/
		change_world(prev_world, world);

		if(!is_anybody_alive(world))
			printf("Everybody died!!!\n");
		if(!is_not_equal(prev_world, world))
			printf("World doesn't change!!!\n");
		usleep(100000);
	} while(is_anybody_alive(world) && is_not_equal(prev_world, world));

	return 0;
}