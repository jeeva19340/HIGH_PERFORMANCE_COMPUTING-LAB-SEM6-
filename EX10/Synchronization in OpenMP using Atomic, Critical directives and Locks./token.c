#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<omp.h>

#define MAX_QUEUE 100
#define MAX_LINE 256

char queue[MAX_QUEUE][MAX_LINE];
int front=0,rear=0,count=0;
omp_lock_t lock;

void enQueue(char* line){
     omp_set_lock(&lock);
     if(count < MAX_QUEUE){
        strcpy(queue[rear],line);
        rear = (rear+1)%MAX_QUEUE;
        #pragma omp atomic
        count++;
     }
     omp_unset_lock(&lock);
}

int deQueue(char* line){
     int success = 0;
     omp_set_lock(&lock);
     if(count>0){
        strcpy(line,queue[front]);
        front = (front+1)%MAX_QUEUE;
        #pragma omp atomic
        count--;
        success = 1;
     }
     omp_unset_lock(&lock);
     return success;
}

void tokenize(char* line){
     char* saveptr;
     char* token = strtok_r(line," \t\n",&saveptr);
     while(token!= NULL){
         #pragma omp critical
         {
           printf("Token: %s\n",token);
         }
         token = strtok_r(NULL," \t\n",&saveptr);
     }
}

int main(){
   omp_init_lock(&lock);
   int num_threads = 4;
   int done;
   #pragma omp parallel num_threads(num_threads)
   {
     int id=omp_get_thread_num();

     if(id < 2){
        char filename[20];
        sprintf(filename,"file%d.txt",id);
        FILE* fp = fopen(filename,"r");
        if(!fp){
           printf("Cannot open %s\n",filename);
        }
        else{
           char line[MAX_LINE];
           while(fgets(line,MAX_LINE,fp)){
              enQueue(line);
           }
           fclose(fp);
        }
     }

     #pragma omp barrier

     #pragma omp single
     done=1;

     if(id >= 2){
          char line[MAX_LINE];
          while(1){
            if(deQueue(line)){
               tokenize(line);
            }
            else if(done && count==0){
               break;
            }
          }
     }
   }
   omp_destroy_lock(&lock);
}
