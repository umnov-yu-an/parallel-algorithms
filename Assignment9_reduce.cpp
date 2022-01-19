#include <iostream>
#include <mpi.h>
using namespace std;
#define N 1000000

int main(int argc, char** argv)
{
	int rank, size, total_sum, sum;
	int* arr = new int[N];
	double start_time, end_time;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	int part = N / size;
	int* arr_part = new int[part];
	
	if (rank == 0) for (int i = 0; i < N; i++) arr[i] = 1;

	if (rank == 0) start_time = MPI_Wtime();
	MPI_Bcast(&part, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(arr, part, MPI_INT, arr_part, part, MPI_INT, 0, MPI_COMM_WORLD);

	total_sum = 0;
	for (int i = 0; i < part; i++) total_sum += arr_part[i];

	MPI_Reduce(&total_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		end_time = MPI_Wtime();
		cout << "Reduce total sum = " << total_sum << endl;
		cout << "Reduce execution time of " << size << " processes = " << end_time - start_time << endl;
	}

	MPI_Finalize();
	return 0;
}