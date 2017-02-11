#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <string.h>
#define L 5
#define l 2
#define d 1
#define h 0.009765625
#define V0 100
#define M 512

int main(int argc, char **argv){
  int rank,size, source, destination, in_number, out_number;

  // Initialize MPI environment
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  // Define the M*M parallel matrix 
  int N,col;
  N=2*M*M;
  col=M/size;

  double **u1,**u2;
  u1=(double **)malloc(M*sizeof(double *));
  u2=(double **)malloc(M*sizeof(double *));
  int i;
  for(i=0;i<M;i++){
    u1[i]=(double *)malloc(col*sizeof(double));
    u2[i]=(double *)malloc(col*sizeof(double));
  } 

  // Define the indexes for which the potential will be held constant
  int j1,j2,iu,id;
  j1=(0.5*(L-l))/h;
  j2=((0.5*(L-l))+l)/h;
  iu=(0.5*(L-d))/h;
  id=((0.5*(L-d))+d)/h;

  // Define the indexes globally
  int start,end;
  start=rank*col;
  end=(rank+1)*col+1;
  if(rank==(size-1)){
    end=(rank+1)*(M/size);
  }
  printf("I am processor %d, and I have columns %d to %d \n",rank,start,end);

  // Initialize the matrices with the corresponding potentials
  int j;
  for(i=0;i<col;i++){
    for(j=0;j<M;j++){
      u2[i][j]=0;
      if(i+start==iu && j>=j1 && j<=j2){
	u1[i][j]=-1*V0/2;
      }
      else if(i+start==id && j>=j1 && j<=j2){
	u1[i][j]=V0/2;
      }
      else{
	u1[i][j]=0;
      }
    }
  }

  
  
  // Write to files
  FILE *file1;
  char buf[10];
  sprintf(buf, "%d.txt", rank);

  file1=fopen(buf,"w");

  for(i=0;i<M;i++){
    for(j=0;j<M;j++){
      fprintf(file1,"%f ",u1[i][j]);
    }
    fprintf(file1,"\n");
  }
  fclose(file1);


  // Finalize the MPI environment
  MPI_Finalize();

  return 0;

  }