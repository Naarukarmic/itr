#include <mpi.h>
#include <stdio.h>
#define PING_PONG_LIMIT 10

int main(int argc, char **argv)
{
	int rank;
	int size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	double start = MPI_Wtime();
	int count[100000]= {0};
	int partner_rank = (rank + 1) % 2;
	while(count[0] < PING_PONG_LIMIT) {
	  if(rank == count[0] % 2) {
	    count[0]++;
	    MPI_Send(&count, 10, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
	    printf("%d sent count %d to %d\n", rank, count[0], partner_rank);
	  } else {
	    MPI_Recv(&count, 10, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    printf("%d received count %d from %d\n", rank, count[0], partner_rank);
	  }
	}
	double end = MPI_Wtime();
	printf("Time for %d is: %d\n", rank, end-start);
	MPI_Finalize();
}
