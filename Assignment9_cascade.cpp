#include <iostream>
#include <mpi.h>
using namespace std;
#define N 1000000

int main(int argc, char** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	int part = N / size;
	int total_sum = 0;
	double start_time, end_time;

	int* arr = new int[N];
	if (rank == 0) for (int i = 0; i < N; i++) arr[i] = 1;

	MPI_Bcast(&part, 1, MPI_INT, 0, MPI_COMM_WORLD);
	int* arr_part = new int[part];
	MPI_Scatter(arr, part, MPI_INT, arr_part, part, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < part; i++) total_sum += arr_part[i];

	if (rank == 0) start_time = MPI_Wtime();
	for (int reduce_order = 2; reduce_order <= size; reduce_order *= 2) {
		if (rank % reduce_order == 0) {
			if (rank + reduce_order / 2 < size) {
				int temp;
				MPI_Recv(&temp, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
				total_sum += temp;
			}
		}
		else {
			MPI_Send(&total_sum, 1, MPI_INT, rank - (rank % reduce_order), 0, MPI_COMM_WORLD);
			break;
		}
	}

	if (rank == 0) {
		end_time = MPI_Wtime();
		cout << "Cascade total sum = " << total_sum << endl;
		cout << "Cascade execution time of " << size << " processes = " << end_time - start_time << endl;
	}

	MPI_Finalize();
	return 0;
}