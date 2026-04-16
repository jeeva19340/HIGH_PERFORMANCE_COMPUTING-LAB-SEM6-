
2.Write an MPI program to create n processes . Let Odd ranked process send a string to master process with message type 1. Let even ranked process send a string to master process with message type 2. Master process will convert the string to uppercase if it is a Type 1 message. It checks if the string is palindrome for a Type 2 message. Prints the result along with rank of the process.


#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<mpi.h>
#define MAX_STRING 1000
int isPalindrome(char str[]){
        int i=0; int j=strlen(str)-1;
        while(i<j){
                if(str[i]!=str[j]){
                        return 0;
                }
                i++;
                j--;
        }
        return 1;
}
void toUppercase(char str[]){
        for(int i=0;str[i]!='\0';i++){
                str[i]=toupper(str[i]);
        }
}
int main(void){
        int rank,size;
        char msg[MAX_STRING];
        MPI_Status status;
        MPI_Init(NULL,NULL);
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);
        MPI_Comm_size(MPI_COMM_WORLD,&size);
        if(rank==1){
                sprintf(msg,"hello");
                MPI_Send(msg,strlen(msg)+1,MPI_CHAR,0,1,MPI_COMM_WORLD);

        }
        else if(rank==2){
                sprintf(msg,"Good");
                MPI_Send(msg,strlen(msg)+1,MPI_CHAR,0,2,MPI_COMM_WORLD);
        }
        else if(rank==3){
                sprintf(msg,"book");
                MPI_Send(msg,strlen(msg)+1,MPI_CHAR,0,1,MPI_COMM_WORLD);
        }
        else if(rank==4){
                sprintf(msg,"madam");
                MPI_Send(msg,strlen(msg)+1,MPI_CHAR,0,2,MPI_COMM_WORLD);
        }
        else{
                for(int i=1;i<size;i++){
                        MPI_Recv(msg,MAX_STRING,MPI_CHAR,i,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
                        if(status.MPI_TAG==1){
                                toUppercase(msg);
                                printf("FROM %d uppercase :%s \n",status.MPI_SOURCE,msg);
                        }
                        else if(status.MPI_TAG == 2){
                                if(isPalindrome(msg)){
                                        printf("FROM %d %s palindrome \n",status.MPI_SOURCE,msg);
                                }
                                else{
                                        printf("FROM %d %s is not palindrome \n",status.MPI_SOURCE,msg);
                        }
                }
        }
        }
MPI_Finalize();
}
