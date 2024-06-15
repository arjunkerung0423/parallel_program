#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void bubble_sort(int* array, int size) {
    int temp;
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1 - i; j++) {
            if (array[j] > array[j + 1]) {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

void printArrayWithRank(int* array, int size, int rank) {
    printf("Rank %d: [", rank);
    for (int i = 0; i < size; i++) {
        printf("%d", array[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

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
    int remainder = array_size % size;

    // Calculate the start and end indices for each process
    int start_index = rank * subset_size + (rank < remainder ? rank : remainder);
    int end_index = start_index + subset_size + (rank < remainder ? 1 : 0);

    // Allocate memory for the local subset
    int local_size = end_index - start_index;
    int* local_array = (int*)malloc(local_size * sizeof(int));

    // Scatter the array to all processes (using manual scatter because of non-uniform chunks)
    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            int start_idx = i * subset_size + (i < remainder ? i : remainder);
            int end_idx = start_idx + subset_size + (i < remainder ? 1 : 0);
            if (i != 0) {
                MPI_Send(array + start_idx, end_idx - start_idx, MPI_INT, i, 0, MPI_COMM_WORLD);
            } else {
                for (int j = start_idx; j < end_idx; j++) {
                    local_array[j - start_idx] = array[j];
                }
            }
        }
    } else {
        MPI_Recv(local_array, local_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Each process performs bubble sort on its local subset
    bubble_sort(local_array, local_size);

    // Display the sorted local array with rank
    printArrayWithRank(local_array, local_size, rank);

    // Gather the sorted subsets back to the root process (using manual gather)
    int* sorted_array = NULL;
    if (rank == 0) {
        sorted_array = (int*)malloc(array_size * sizeof(int));
        for (int i = 0; i < local_size; i++) {
            sorted_array[i] = local_array[i];
        }
        for (int i = 1; i < size; i++) {
            int start_idx = i * subset_size + (i < remainder ? i : remainder);
            int end_idx = start_idx + subset_size + (i < remainder ? 1 : 0);
            MPI_Recv(sorted_array + start_idx, end_idx - start_idx, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Print the fully gathered array before final sort
        printf("Gathered array before final sort: ");
        printArrayWithRank(sorted_array, array_size, rank);

        // Perform the final bubble sort on the gathered array
        bubble_sort(sorted_array, array_size);

        // Print the fully sorted array
        printf("Fully sorted array: ");
        printArrayWithRank(sorted_array, array_size, rank);

        free(sorted_array);
    } else {
        MPI_Send(local_array, local_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    free(local_array);
    MPI_Finalize();
    return 0;
}
