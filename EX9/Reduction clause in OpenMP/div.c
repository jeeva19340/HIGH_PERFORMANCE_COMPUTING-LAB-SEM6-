#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main(int argc,char* argv[]){

    int m,n,i,j,k,add=0;
    printf("Enter the Number of rows and Columns:");
    scanf("%d %d",&m,&n);

    long thread_count = strtol(argv[1],NULL,10);

    int **mat = (int**)malloc(m*sizeof(int*));
    for(i=0;i<n;i++){
        mat[i] = (int*)malloc(n*sizeof(int));
    }

    printf("Enter Matrix Elements:\n");
    for(i=0;i<m;i++){
       for(j=0;j<n;j++){
           scanf("%d",&mat[i][j]);
       }
    }

    printf("Enter k value:");
    scanf("%d",&k);

#pragma omp parallel for num_threads(thread_count) reduction(+:add)
    for(i=0;i<m;i++){
       for(j=0;j<n;j++){
           if(mat[i][j]%k == 0){
              add += mat[i][j];
           }
       }
    }

    printf("Sum of all elements divisible by %d : %d\n",k,add);
    return 0;
}
