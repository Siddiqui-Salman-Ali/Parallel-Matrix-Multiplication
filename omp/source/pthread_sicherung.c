typedef struct{
  int tid;
  matrix* matrixA;
  matrix* matrixB; 
  matrix* matrixC;
}thread_args; 

pthread_mutex_t mutex;


int numOfThread=10;

void *threadFunction(void *data){
    thread_args* args=(thread_args*) data; ;
	
    int TID = args->tid;

    int colArr[numOfThread];
    int addArr[numOfThread];
    int tempSum[numOfThread];
    
    //matrix A=*args->matrixA;
   // matrix B=*args->matrixB;
    //matrix C=*args->matrixC;
    
    int numOfIters = args->matrixA->rows / numOfThread;
	printf("--\nOKOKOK\n");
    int lastNumOfIters = numOfIters + args->matrixA->rows - (numOfIters * numOfThread);
    int startIters = numOfIters*TID;
    
    if(TID==numOfThread-1){
        numOfIters=lastNumOfIters;
    }

    int endIters=startIters+numOfIters;
    printf("Thread: %d Anzahl: %d %d %d\n",TID, numOfIters,startIters, endIters);
    
    for(row = startIters ; row < endIters ; row++){
    //printf("\nI am now in row %d\n",row);
	//printf("\nI am now in col ");

        for(colArr[args->tid] = 0 ; colArr[args->tid] < args->matrixB->cols ; colArr[args->tid]++ ){

    	//printf("%d",colArr[args->tid]);

	tempSum[args->tid]=0;
            for( addArr[args->tid] = 0 ; addArr[args->tid] < args->matrixA->cols ; addArr[args->tid]++ ){
            
                tempSum[args->tid] += (int) args->matrixA->matrix[row][addArr[args->tid]] * (int) args->matrixB->matrix[addArr[args->tid]][colArr[args->tid]];
            }

	    //if (tempSum == 0){printf("(%d) ",tempSum[args->tid]);}else{ printf(" ");}	

            pthread_mutex_lock(&mutex);
            args->matrixC->matrix[row][colArr[args->tid]] = tempSum[args->tid]; 
	    pthread_mutex_unlock(&mutex);
        }
//printf("\n ");
    }
    
    pthread_exit(NULL);

}


multiReturn matrixMultiPTH(matrix matrixA, matrix matrixB, matrix matrixC){
 printf("\nStart parallel multiplication with pthreads ... \n");
 double time1=omp_get_wtime();
  
  int t;
  pthread_t threads[numOfThread];
  thread_args args[numOfThread];
 
  pthread_mutex_init(&mutex,NULL);
  
  for(t=0;t<numOfThread;t++){
        args[t].tid=t;
        args[t].matrixA=&matrixA;
        args[t].matrixB=&matrixB;
        args[t].matrixC=&matrixC;
		pthread_create(&threads[t], NULL, threadFunction, (void *) &args[t]);	
  }
  for(t=0;t<numOfThread;t++){
		pthread_join(threads[t], NULL);
  }
  printf("Fertig...\n");
  
  double time2=omp_get_wtime();
  printf("finished\n\n");
  
  multiReturn mr = {"PTHREAD",time2-time1};
  
  return mr;
}







void *threadFunction2(void *data){
    thread_args* args=(thread_args*) data; ;
    
    
    printf("\n\nhmmmm: %d %s\n\n",args->matrixC->matrix[10][10],args->matrixC->name);
    
    int numOfIters=args->matrixA->rows/numOfThread;
    int lastNumOfIters=numOfIters+args->matrixA->rows-numOfIters*numOfThread;
    int startIters = numOfIters*args->tid;
    
    if(args->tid==numOfThread-1){
        numOfIters=lastNumOfIters;
    }
    printf("Thread: %d Anzahl: %d %d %d\n",args->tid, numOfIters,startIters, startIters+numOfIters);
    
  
    //printf("\nblub: %d\n",A.matrix[10][10]);
    
    pthread_exit(NULL);

}



multiReturn matrixMultiPTH2(matrix matrixA, matrix matrixB, matrix matrixC){
 printf("\nStart parallel multiplication with pthreads ... \n");
 double time1=omp_get_wtime();
  
  int t;
  pthread_t threads[numOfThread];
  thread_args args[numOfThread];
 
  pthread_mutex_init(&mutex,NULL);
  
  for(t=0;t<numOfThread;t++){
        args[t].tid=t;
        args[t].matrixA=&matrixA;
        args[t].matrixB=&matrixB;
        args[t].matrixC=&matrixC;
		pthread_create(&threads[t], NULL, threadFunction2, (void *) &args[t]);	
  }
  for(t=0;t<numOfThread;t++){
		pthread_join(threads[t], NULL);
  }
  printf("Fertig...\n");
  
  double time2=omp_get_wtime();
  printf("finished\n\n");
  
  multiReturn mr = {"PTHREAD",time2-time1};
  
  return mr;
}


