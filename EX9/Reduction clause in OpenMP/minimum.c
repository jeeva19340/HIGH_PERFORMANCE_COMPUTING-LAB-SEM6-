#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<limits.h>

int main(int argc,char* argv){
    int m,n,count=0,i,j,k;
    printf("Enter rows and Columns of Matrix:");
    scanf("%d %d",&m,&n);
    int **mat = (int**)malloc(m*sizeof(int*));
    for(i=0;i<m;i++){
        mat[i] = (int*)malloc(n*sizeof(int));
    }
    printf("Enter Matrix elements:\n");
    for(i=0;i<m;i++){
       for(j=0;j<n;j++){
          scanf("%d",&mat[i][j]);
       }
    }

#pragma omp parallel for num_threads(thread_count)\reduction (+:count) private(j,k)
    for(i=0;i<n;i++){
       int min = INT_MAX;
       int max = 0;
       int index=0;
       for(j=0;j<m;j++){
          if(max < mat[j][i]){
             max = mat[j][i];
             index = j;
          }
       }
       for(k=0;k<m;k++){
          if(min > mat[index][k]){
             min = mat[index][k];
          }
       }
       if(min == max){
           count++;
       }
    }
    printf("Saddle point count of matrix: %d\n",count);
    return 0;
}
