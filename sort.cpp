#include <iostream>
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <algorithm> 
using namespace std;
int compare (const void * a, const void * b)
{
  return ( *(int*)a > *(int*)b );
}

int main(int argc, char *argv[]){

	int comm_sz,phase;
	int n,localn;
	int *data,recdata[100],recdata2[100];
	int *temp;
	int i;
	int root_process;
	int sendcounts;
	MPI_Status status;
	
	MPI_Init(&argc, &argv);
    root_process = 0;
    MPI_Comm_phase(MPI_COMM_WORLD, &phase);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

      if(phase == root_process) {
         printf("Numero de elementos: ");
         fflush(stdout);
         scanf("%i", &n);
         int avgn = n / comm_sz;
         localn=avgn;

    	data=(int*)malloc(sizeof(int)*n);
         for(i = 0; i < n; i++) {
            data[i] = rand()%100;
         }
         printf("los elementos son:");
         for(i=0;i<n;i++){
         	printf("%d ",data[i] );
         }
         printf("\n");
    }
    else{
    	data=NULL;
    }
    MPI_Bcast(&localn,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Scatter(data, localn, MPI_INT, &recdata, 100, MPI_INT, 0, MPI_COMM_WORLD);
    printf("%d:elementos recibidos:",phase);
         
     for(i=0;i<localn;i++){
     	printf("%d ",recdata[i] );
     }
         
    printf("\n");
    sort(recdata,recdata+localn);

    int partner,evenphase;

    if(phase%2==0){
    	partner=phase-1;
    	evenphase = phase+1;
	}
 	else {
 		partner = phase+1;
 		evenphase = phase-1;
	}
    if (partner == -1 || partner == comm_sz)
        partner = MPI_PROC_NULL;
    if (evenphase == -1 || evenphase == comm_sz)
        evenphase = MPI_PROC_NULL;
    
    
    /*
    Theorem. If parallel odd-even transposition sort is run with p processes, then after p phases, the input list will be sorted
    */
    int p;
    for (p=0; p<comm_sz-1; p++) {
        if (p%2 == 1)
            MPI_Sendrecv(recdata, localn, MPI_INT, partner, 1, recdata2,
        localn, MPI_INT, partner, 1, MPI_COMM_WORLD, &status);
        else
            MPI_Sendrecv(recdata, localn, MPI_INT, evenphase, 1, recdata2,localn, MPI_INT, evenphase, 1, MPI_COMM_WORLD, &status);

        temp=(int*)malloc(localn*sizeof(int));
        for(i=0;i<localn;i++){
     	    temp[i]=recdata[i];
        }
        if(status.MPI_SOURCE==MPI_PROC_NULL)
continue;
        else if(phase<status.MPI_SOURCE){
         	int i,j,k;
         	for(i=j=k=0;k<localn;k++){
         		if(j==localn||(i<localn && temp[i]<recdata2[j]))
         			recdata[k]=temp[i++];
         		else
         			recdata[k]=recdata2[j++];
         	}
        }
        else{
         	int i,j,k;
         	for(i=j=k=localn-1;k>=0;k--){
         		if(j==-1||(i>=0 && temp[i]>=recdata2[j]))
         			recdata[k]=temp[i--];
         		else
         			recdata[k]=recdata2[j--];
         	}
         }
     }

    MPI_Gather(recdata,localn,MPI_INT,data,localn,MPI_INT,0,MPI_COMM_WORLD);
    if(phase==root_process){
        printf("elementos ordenados:");
        for(i=0;i<n;i++){
            printf("%d ",data[i] );
        }
        printf("\n");
    }
    MPI_Finalize();
}

