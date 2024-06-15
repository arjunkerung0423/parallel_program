#include<stdio.h>
#include<mpi.h>
#include<time.h>
#include<stdlib.h>
void oddeven(int *arr,int n){
    for(int i=0;i<n-1;i++){
        if(i%2!=0){
            for(int j = 0;j<n;j++){
                if(j%2!=0){
                    if(arr[j]>arr[j+1]){
                        int temp = arr[j];
                        arr[j] = arr[j+1];
                        arr[j+1] = temp;
                    }
                }
            }
        }
        else {
               for(int j = 0;j<n;j++){
                if(j%2==0){
                    if(arr[j]>arr[j+1]){
                        int temp = arr[j];
                        arr[j] = arr[j+1];
                        arr[j+1] = temp;
                    }
                }
            }
            }
        }
    }
int main(int argc,char *argv[]){
    int rank,size;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    double start,end;

    int n = 8;
    int *arr = NULL;

    int *subarr = NULL;
      int local = n/size;

    if(rank == 0){
        arr = (int*)malloc(n*sizeof(int));
        for(int i =0;i<n;i++){
            arr[i] = rand()%10;
        }
        printf("\nInitial array: ");
         for(int i =0;i<n;i++){
            printf("%d ",arr[i]);
        }
    }
    subarr = (int *)malloc(n*sizeof(int));
    MPI_Scatter(arr,local,MPI_INT,subarr,local,MPI_INT,0,MPI_COMM_WORLD);
       for(int i =0;i<size;i++){
        if(i == rank){
            printf("\nRANK %d Local array: ",rank);
            for(int j =0;j<local;j++){
                printf("%d ",subarr[j]);
            }
        }
    }
    start = MPI_Wtime();
    oddeven(subarr,local);
    end = MPI_Wtime();

     for(int i =0;i<size;i++){
        if(i == rank){
            printf("\nRANK %dSorted Local array: ",rank);
            for(int j =0;j<local;j++){
                printf("%d ",subarr[j]);
            }
        }
    }
    MPI_Gather(subarr,local,MPI_INT,arr,local,MPI_INT,0,MPI_COMM_WORLD);
    if(rank == 0){
        printf("\nGathered Array at Root: ");
            for(int j =0;j<n;j++){
                printf("%d ",arr[j]);
            }
                oddeven(arr,n);
            printf("\nGathered Array at Root after last sort: ");
            for(int j =0;j<n;j++){
                printf("%d ",arr[j]);
            }
            printf("\nTime Taken %lf s",end - start);
    }
    free(arr);
    free(subarr);
    MPI_Finalize();
    return 0;

}