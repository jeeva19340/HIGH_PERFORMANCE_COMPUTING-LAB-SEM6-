#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

double f(double x){
    return x*x;
}

void calculateTrapezoid(double a,double b,int n,int thread_count,double *global_result){
     double x,h,sum=0.0;
     int i;

     h = (b-a)/n;
     sum = (f(a)+f(b))/2.0;

#pragma omp parallel for num_threads(thread_count)\reduction (+:sum)

     for(i=1;i<n;i++){
        x = a + (i*h);
        sum += f(x);
     }

     sum = sum*h;
     *global_result = sum;
}

int main(int argc,char* argv[]){

    double global_res = 0.0;
    double a,b;
    int n,thread_count;

    thread_count = strtol(argv[1],NULL,10);

    printf("Enter Limits:\n");
    printf("Enter a,b :");
    scanf("%lf %lf",&a,&b);

    printf("Enter the Number of trapezoids:");
    scanf("%d",&n);

    calculateTrapezoid(a,b,n,thread_count,&global_res);

    printf("With n = %d trapezoids, our estimate",n);
    printf(" of the integral from %.4f to %.4f = %.4f\n",a,b,global_res);

    return 0;
}
