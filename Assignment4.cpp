#include <iostream>
#include "mpi.h"
using namespace std;
int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	int rank, n, i, message, j;
	double start_time, end_time;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &n);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	start_time = MPI_Wtime();
	if (rank == 2)
	{
		cout << "Hello from process " << rank << "\n";
		for (i = 1; i < n; i++) {
			MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			cout << "Hello from process " << message << " to process " << rank << endl;
		}
	}
	else MPI_Send(&rank, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
	end_time = MPI_Wtime();
	cout << "Execution time of process " << rank << " = " << (end_time - start_time) / NTIMES << endl;
	MPI_Finalize();
	return 0;
}