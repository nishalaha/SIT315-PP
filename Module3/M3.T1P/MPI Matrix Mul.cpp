#include<mpi.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include <sys/time.h>
#include <time.h>

#define T 900

using namespace std;

void AnalyseSet(int set[T][T]);
void printSet(int set[T][T]);
void MatrixMultiplication(int np, int rank, int inputArray1[T][T], int inputArray2[T][T], int outputArray[T][T]);

struct timeval check;

void printOutput(int outputArray[N]){
    for (int i = 0 ; i < N; i++){
        printf(" %d :", outputArray[i]);
    }
    
}


int main(){
    MPI_Init(NULL, NULL);

    int np = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &np);     //number of nodes

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int inputArray1[N][N], inputArray2[N][N];
    int outputArray[N][N]={{0}};
    //int outputArray[N*N]={0};

    if (rank==0) {
        AnalyseSet(inputArray1);
        AnalyseSet(inputArray2);

           }else{
        //printArrays(outputArray);
    }

    //Timer Start
    if (rank == 0){
        printf("MPI Matrix Multiplication.\n");
    }

    gettimeofday(&check, NULL);
	long timeofday_start = (long)check.tv_sec * 1000 + (long)check.tv_usec /1000;

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&inputArray1, N*N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&inputArray2, N*N, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    MatrixMultiplication(np, rank, inputArray1, inputArray2, outputArray);
    MPI_Barrier(MPI_COMM_WORLD);
    
    //if(rank==0)printArrays(outputArray);
    
    MPI_Finalize();

    //Timer end
    gettimeofday(&check, NULL);
	long timeofday_end = (long)check.tv_sec * 1000 + (long)check.tv_usec /1000;
	double time_elapsed = timeofday_end - timeofday_start;
	
    if (rank == 0){
        printf("\t\tTime elapsed: %f ms\n", time_elapsed);
    }

    return 0;
}

void AnalyseSet(int set[T][T]) {
	printf("intialising array... ");
	for (int i = 0; i < T; i++)
	{
		for (int j = 0; j < T; j++)
		{
			set[i][j] = rand() % ((10 - 1) + 1) + 1;
		}
	}
	printf("complete\n");
}		//intialises array with random values, uses the N global variable

void printSet(int set[T][T]){
	printf("[");
	for (int i = 0; i < T; i++) {
		printf("[");
		for (int j = 0; j < T; j++) {
			printf("%i", set[i][j]);
			printf(" ");
		}
		printf("]\n");
	}
	printf("]\n\n");
}		//prints array to console

void MatrixMultiplication(int np, int rank, int inputArray1[T][T], int inputArray2[T][T], int outputArray[T][T]){
    long value;
    int range = T/np;
	int start = rank * range;
	int end = start + range;
    int buffArray[range][T]={0};
  
    for (int i = start ; i < end ; i++)
	{
		for (int j = 0; j < T; j++)
		{
			value = 0;
			for (int k = 0; k < T; k++)
			{
				value += inputArray1[i][k] * inputArray2[k][j];
			}
            buffArray[i - start][j]=value;
		}
	}
   
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(buffArray, range * T, MPI_INT, outputArray, range * T, MPI_INT, 0, MPI_COMM_WORLD);
}