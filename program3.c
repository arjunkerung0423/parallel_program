#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define VECTOR_SIZE 6

// Function to perform vector addition
void vectorAddition(int *vec1, int *vec2, int *result, int size) {
    for (int i = 0; i < size; i++) {
        result[i] = vec1[i] + vec2[i];
    }
}

// Function to perform vector multiplication
void vectorMultiplication(int *vec1, int *vec2, int *result, int size) {
    for (int i = 0; i < size; i++) {
        result[i] = vec1[i] * vec2[i];
    }
}

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *vec1 = NULL, *vec2 = NULL, *result = NULL;
    int*lv =NULL, *lv2 = NULL, *presult = NULL;
    vec1 = (int *)malloc(VECTOR_SIZE * sizeof(int));
    vec2 = (int *)malloc(VECTOR_SIZE * sizeof(int));
    result = (int *)malloc(VECTOR_SIZE * sizeof(int));

    srand(time(0) + rank); // Ensure different seeds for different ranks

    if (rank == 0) {
        // Initialize vectors
        printf("Vector 1: ");
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vec1[i] = rand() % 10;
            printf("%d ", vec1[i]);
        }
        printf("\n");

        printf("Vector 2: ");
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vec2[i] = rand() % 100;
            printf("%d ", vec2[i]);
        }
        printf("\n");
    }

    double start_time, end_time;
    int local = VECTOR_SIZE/size;
    presult = (int*)malloc(local*sizeof(int));
    lv = (int*)malloc(local*sizeof(int));
    lv2 = (int*)malloc(local*sizeof(int));
    // Broadcast vectors to all processes
    MPI_Scatter(vec1,local,MPI_INT,lv,local,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Scatter(vec2,local,MPI_INT,lv2,local,MPI_INT,0,MPI_COMM_WORLD);
    //MPI_Bcast(vec1, VECTOR_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    //MPI_Bcast(vec2, VECTOR_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    // Parallel execution of vector addition

        for(int i = 0;i<size;i++){
        if(rank == i){
                printf("\nRANK %d local arr1: ",rank);
            for(int j = 0;j<local;j++){
                printf(" %d",lv[j]);
            }
            printf("\nRANK %d local arr2: ",rank);
            for(int j = 0;j<local;j++){
                printf(" %d",lv2[j]);
            }
        }
    }

    start_time = MPI_Wtime();
    vectorAddition(lv, lv2, presult, local);
    MPI_Gather(presult,local,MPI_INT,result,local,MPI_INT,0,MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    if (rank == 0) {
        printf("\nVector Addition Result: ");
        for (int i = 0; i < VECTOR_SIZE; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        printf("Execution time for vector addition (parallel): %lf seconds\n", end_time - start_time);
    }

    // Parallel execution of vector multiplication
    start_time = MPI_Wtime();
    vectorMultiplication(lv, lv2, presult, local);
    MPI_Gather(presult,local,MPI_INT,result,local,MPI_INT,0,MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Vector Multiplication Result: ");
        for (int i = 0; i < VECTOR_SIZE; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        printf("Execution time for vector multiplication (parallel): %lf seconds\n", end_time - start_time);
    }

    free(vec1);
    free(vec2);
    free(result);
    MPI_Finalize();
    return 0;
}