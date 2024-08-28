#include<stdio.h>
#include<stdlib.h>
#include<veda.h>
#include<time.h>
#include<math.h>

#define NOW() ({ struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); ts; })
#define TIME(start, end) ((double)(end.tv_sec - start.tv_sec) * 1000.0 + (double)(end.tv_nsec - start.tv_nsec) / 1000000.0)

#define VEDA(err) check(err, __FILE__, __LINE__)

void check(VEDAresult err, const char* file, const int line) {
	if(err != VEDA_SUCCESS) {
		const char *name, *str;
		vedaGetErrorName	(err, &name);
		vedaGetErrorString	(err, &str);
		printf("%s: %s @ %s:%i\n", name, str, file, line);
		exit(1);
	}
}

double* getDArray(int);
void printDArray(double*,int);

int main(int argc,char *argv[]){
    int t = atoi(argv[1]);
   
        struct timespec start, end;
        start = NOW();

        VEDA(vedaInit(0));

        srand(time(NULL));
        
        int len=t;

        double *array_a = getDArray(len);
        double *t_taken = (double*)malloc(sizeof(double));
        t_taken[0] = 0.0;

        //printDArray(array_a,len);

        size_t matrix_size = len*sizeof(double);

        VEDAcontext ctx;
        VEDAdevice vd;
        VEDA(vedaDeviceGet(&vd,2));
        VEDA(vedaCtxCreate(&ctx,1,vd));
        VEDA(vedaCtxPushCurrent(ctx));

        VEDAmodule mod;
        VEDA(vedaModuleLoad(&mod,"lib_device.vso"));

        VEDAfunction func;
        VEDA(vedaModuleGetFunction(&func,mod,"swap_max"));

        VEDAdeviceptr a,time_taken;
        VEDA(vedaMemAllocAsync(&a,matrix_size,0));
        VEDA(vedaMemAllocAsync(&time_taken,sizeof(double),0));

        VEDA(vedaMemcpyHtoDAsync(a,array_a,matrix_size,0));
        VEDA(vedaMemcpyHtoDAsync(time_taken,t_taken,sizeof(double),0));

        VEDAargs args;
        VEDA(vedaArgsCreate(&args));
        VEDA(vedaArgsSetVPtr(args,0,a));
        VEDA(vedaArgsSetVPtr(args,1,time_taken));

        for(int i=0;i<len-1;i++){
            VEDA(vedaArgsSetI64(args,2,len-i));
            VEDA(vedaLaunchKernel(func,0,args));
        }

        VEDA(vedaCtxSynchronize());

        VEDA(vedaArgsDestroy(args));

        VEDA(vedaMemcpyDtoHAsync(array_a,a,matrix_size,0));
        VEDA(vedaMemcpyDtoHAsync(t_taken,time_taken,sizeof(double),0));


        VEDA(vedaMemFreeAsync(a,0));
        VEDA(vedaMemFreeAsync(time_taken,0));

        VEDA(vedaExit());

        //printDArray(array_a,len);

        //print2DMatrix(array_a,row,col);
        end = NOW();
        double elapsed = TIME(start, end);
        printf("%d,%lf \n",len,t_taken[0]);
    
        //printf("Total Host Elapsed Time: %f ms\n", elapsed);

    return 0;
}

double* getDArray(int len){

    double *m1 = (double*)malloc(len*sizeof(double));

    for(int i=0;i<len;i++){
            *(m1+i) = (double)rand()/RAND_MAX;
    }

    return m1;

}

void printDArray(double *matrix,int len){
    for(int i=0;i<len;i++){
        printf(" %lf ",*(matrix+i));
    }
    printf("\n");
}