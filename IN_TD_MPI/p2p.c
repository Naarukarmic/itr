#include <mpi.h>
#include <stdio.h>
#define PING_PONG_LIMIT 100

int main(int argc, char **argv)
{
	int rank;
	int size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	double time = 0.0;
	time -= MPI_Wtime();
	int count = 0;
	
	while(count < PING_PONG_LIMIT) {
	  if(rank == 0) {
	    count++;
	    int i;
	    for(i = 0; i < size; i++) {
	      if(i != 0) {
	        MPI_Send(&count, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
	        printf("%d sent count %d to %d\n", rank, count, i);
	      }
	    }
	  } else {
	    MPI_Recv(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    printf("%d received count %d from root\n", rank, count);
	  }
	}
	time += MPI_Wtime();
	printf("Time for %d is: %f\n", rank, time);
	MPI_Finalize();
}
