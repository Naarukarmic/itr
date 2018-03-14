#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int id;
	int size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	printf("Hello world ! I am process %d / %d \n", id, size);
	MPI_Finalize();
}
