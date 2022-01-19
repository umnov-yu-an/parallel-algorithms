#include <iostream>
#include "mpi.h"
#include <ctime>
using namespace std;
#define N 500

int main(int argc, char **argv)
{
    srand(time(NULL));
    int rank, size;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int** A = new int* [size];
    int** B = new int* [size];
    int** C = new int* [size];
    for (int i = 0; i < size; i++) {
        A[i] = new int[size];
        B[i] = new int[size];
        C[i] = new int[size];
    }
    for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                A[i][j] = rand() % 10;
                B[i][j] = rand() % 10;
                C[i][j] = 0;
            }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            for(int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    end_time = MPI_Wtime();
    cout << "Process " << rank << " -> time = " << end_time - start_time << endl;
    MPI_Finalize();

    return 0;
}

