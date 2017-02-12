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
  MPI_Request send_request, recv_request;
  MPI_Status status;

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
    u1[i]=(double *)malloc((col+2)*sizeof(double));
    u2[i]=(double *)malloc((col+2)*sizeof(double));
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
  for(i=0;i<col+1;i++){
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
 
  // Start of the relaxation method for the potential
  int k;
  double number;
  
  MPI_Barrier(MPI_COMM_WORLD);
  
  for(k=0;k<3;k++){
    for(j=1;j<M;j++){
      if(rank!=size-1){
	number=u1[col][j];
        MPI_Send(&number, 1, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD);
      }
      if(rank!=0){
	MPI_Recv(&number, 1, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	u1[0][j]=number;
	number=u1[1][j];
	MPI_Send(&number, 1, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD);
      }
      if(rank!=size-1){
	MPI_Recv(&number, 1, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	u1[col+1][j]=number;
      }
    }
    
    MPI_Barrier(MPI_COMM_WORLD);

    for(i=1;i<col;i++){
      for(j=1;j<M;j++){
	if(i+start==iu && j>=j1 && j<=j2){
	  u2[i][j]=-1*V0/2;
	}
	else if(i+start==id && j>=j1 && j<=j2){
	  u2[i][j]=V0/2;
	}
	else{
	  u2[i][j]=0.25*(u1[i+1][j]+u2[i-1][j]+u1[i][j+1]+u2[i][j-1]);
	}
      }
    }
    
    for(i=0;i<col+2;i++){
      for(j=0;j<M;j++){
	u1[i][j]=u2[i][j];
      }
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }
  
  printf("Empezar a escribir archivos: Proceso %d \n",rank);
  // Write to files the potential
  FILE *file1,*file2,*file3;
  char buf[10];
  char baf[10];
  char bef[10];
  sprintf(buf, "v%d.txt", rank);
  sprintf(baf, "ex%d.txt", rank);
  sprintf(bef, "ey%d.txt", rank);
  file1=fopen(buf,"w");
  file2=fopen(baf,"w");
  file3=fopen(bef,"w");

  for(j=0;j<M;j++){
    for(i=0;i<col+1;i++){
      fprintf(file1,"%f ",u1[i][j]);
    }
    fprintf(file1,"\n");
  }
  fclose(file1);

  MPI_Barrier(MPI_COMM_WORLD);

  //Calculate the potential
  double ex,ey;
  for(j=1;j<M;j++){
    for(i=1;i<col;i++){
      ex=(-1/(2*h))*(u1[i+1][j]-u1[i-1][j]);
      ey=(-1/(2*h))*(u1[i][j+1]-u1[i][j-1]);
      fprintf(file2,"%f ",ex);
      fprintf(file3,"%f ",ey);
    }
    fprintf(file2,"\n");
    fprintf(file3,"\n");
  }
  fclose(file2);
  fclose(file3);
  
  // Finalize the MPI environment
  MPI_Finalize();

  return 0;

  }
