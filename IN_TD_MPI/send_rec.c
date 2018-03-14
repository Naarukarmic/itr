#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int rank;
	int size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	int number;
	if(rank == 0) {
	  number = -1;
	  sleep(5);
	  MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	} else if (rank == 1) {
	  MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  printf("Process %d received number %d from master\n", rank, number);
	}
	MPI_Finalize();
}
