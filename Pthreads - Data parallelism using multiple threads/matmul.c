#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define MAX 10

int A[MAX][MAX],B[MAX][MAX],add[MAX][MAX],mul[MAX][MAX],r1,r2,c1,c2;

void *mat_add(void *arg)
{
   int i,j;
   for(i=0;i<r1;i++)
   {
      for(j=0;j<c1;j++)
      {
         add[i][j]=A[i][j]+B[i][j];
      }
   }
   printf("Addition completed by thread\n");
   pthread_exit(NULL);
}
void *mat_mul(void *arg)
{
   int i,j,k;
   for(i=0;i<r2;i++)
   {
      for(j=0;j<c2;j++)
      {
         mul[i][j]=0;
         for(k=0;k<c1;k++)
         {
            mul[i][j]+=A[i][k]*B[k][j];
         }
      }
   }
   printf("Multiplication completed by thread\n");
   pthread_exit(NULL);
}

int main()
{
   printf("MATRIX MULTIPLICATION AND ADDITION USING THREADS\n");
   pthread_t t1,t2;
   int i,j;
   printf("Enter the no of rows and columns of Matrix A:");
   scanf("%d%d",&r1,&c1);
   printf("Enter the no of rows and columns of Matrix B:");
   scanf("%d%d",&r2,&c2);
   if(c1!=r2)
   {
      printf("Matrix multiplication not possible");
      return 0;
   }
   if(r1!=r2 || c1!=c2)
   {
      printf("Matrix addition not possible");
      return 0;
   }
   printf("Enter elements of matrixA:");
   for(i=0;i<r1;i++)
   {
      for(j=0;j<c1;j++)
      {
         scanf("%d",&A[i][j]);
      }
   }
   printf("Enter elements of matrixB:");
   for(i=0;i<r2;i++)
   {
      for(j=0;j<c2;j++)
      {
         scanf("%d",&B[i][j]);
      }
   }
   pthread_create(&t1,NULL,mat_add,NULL);
   pthread_create(&t2,NULL,mat_mul,NULL);
   pthread_join(t1,NULL);
   pthread_join(t2,NULL);
   printf("Matrix addition result:\n");
   for(i=0;i<r1;i++)
   {
      for(j=0;j<c1;j++)
      {
         printf(" %d ",add[i][j]);
      }
      printf("\n");
   }
   printf("Matrix multiplication result:\n");
   for(i=0;i<r2;i++)
   {
      for(j=0;j<c2;j++)
      {
         printf(" %d ",mul[i][j]);
      }
      printf("\n");
   }
   return 0;
}
