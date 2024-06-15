#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the rank of the process
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get the total number of processes

    // Define the array and its size
    int array[] = {34, 15, 89, 2, 78, 45, 19, 4, 22, 31, 44, 91, 13, 5, 18};
    int array_size = sizeof(array) / sizeof(array[0]);

    // Determine the subset size for each process
    int subset_size = array_size / size;
    int start_index = rank * subset_size;
    int end_index = (rank == size - 1) ? array_size : start_index + subset_size;

   // printf("Start index: %d , End Index %d\n", start_index,end_index);

    // Find the minimum value in the local subset
     // Start time
    double start_time = MPI_Wtime();
    int local_min = INT_MAX;
    for (int i = start_index; i < end_index; i++) {
            printf("%d ",array[i]);
        if (array[i] < local_min) {
            local_min = array[i];

        }
    }
    // Print the local minimum value and the rank
    printf("Rank %d: Local minimum value = %d\n", rank, local_min);
     int global_min;
    MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
      double end_time = MPI_Wtime();
    // The root process (rank 0) prints the global minimum value
    if (rank == 0) {
        printf("Global minimum value = %d\n", global_min);

        printf("Execution time = %f seconds\n", end_time - start_time);
    }
    MPI_Finalize();
    return 0;
}
