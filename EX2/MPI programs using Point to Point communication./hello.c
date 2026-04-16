
/*1. Write an MPI program to create n processes and print a hello message with rank from each process.*/

#include<stdio.h>
#include<string.h>
#include<mpi.h>
#define MAX_STRING 100
int main(void){
        int my_rank,comm_sz;
        char message[MAX_STRING];
        MPI_Init(NULL,NULL);
        MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
        MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
        if(my_rank!=0){
                sprintf(message,"HELLO FROM PROCESS %d",my_rank);
                MPI_Send(message,strlen(message)+1,MPI_CHAR,0,0,MPI_COMM_WORLD);
        }
        else{
                printf("HELLO FROM PROCESS %d\n",my_rank);
                for(int i=1;i<comm_sz;i++){
                        MPI_Recv(message,MAX_STRING,MPI_CHAR,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                        printf("%s\n",message);
                }
        }
        MPI_Finalize();
        return 0;
}
