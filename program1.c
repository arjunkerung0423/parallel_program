
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int operand1 = 10;
    int operand2 = 5;
    int result = 0;

    // Perform arithmetic operations based on rank
    switch (rank) {
        case 0:
            result = operand1 + operand2;
            printf("Process %d: Addition result: %d\n", rank, result);
            break;
        case 1:
            result = operand1 * operand2;
            printf("Process %d: Multiplication result: %d\n", rank, result);
            break;
        case 2:
            result = operand1 / operand2;
            printf("Process %d: Division result: %d\n", rank, result);
            break;
        case 3:
            result = operand1 - operand2;
            printf("Process %d: Subtraction result: %d\n", rank, result);
            break;
        default:
            printf("Process %d: No operation assigned\n", rank);


    }


    MPI_Finalize();
    return 0;
}
