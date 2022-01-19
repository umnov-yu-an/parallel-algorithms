#include <iostream>
#include <mpi.h>
#include <ctime>
using namespace std;
#define N 10
#define LENGTH 1000000
int main(int argc, char** argv)
{
	srand(time(NULL));
	double R, T, L;
	int* message = new int[LENGTH];
	double start_time, end_time;
	int size, rank;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		for (int i = 0; i < LENGTH; i++) message[i] = rand() % 26 + 'a';
	}
	for (int i = 0; i <= LENGTH; i *= 10) {
		if (rank == 0) start_time = MPI_Wtime();
		for (int j = 0; j < N; j++) {
			if (rank == 0) {
				MPI_Send(&message[0], i, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
				MPI_Recv(&message[0], i, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &status);
			}
			else {
				MPI_Recv(&message[0], i, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
				MPI_Send(&message[0], i, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
			}
		}
		if (rank == 0) {
			end_time = MPI_Wtime();
			T = end_time - start_time;
			L = i * sizeof(char);
			R = 2 * N * L / T;
			cout << "Length = " << i << ", ";
			if (i == 0) cout << "Latency = " << T / (2 * N) << endl;
			else {
				cout << "Length in bytes = " << L << ", ";
				cout << "Time = " << T / N << ", ";
				cout << "Bandwidth = " << R << endl;
			}
			if (i == 0) i++;
		}
	}
	MPI_Finalize();

	return 0;
}