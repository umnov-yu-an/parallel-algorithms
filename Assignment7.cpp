#include<iostream>
#include"mpi.h"
#define n 1000000
int array1[n];
int array2[n];
using namespace std;

void fillVectors(int len) {
    for (int i = 0; i < len; i++) {
        array1[i] = 1;
        array2[i] = 1;
    }
}

int main (int argc, char *argv[]) 
{
    int start, stop;
    int sBufer = 0;
    MPI_Init(&argc, &argv);
    int rank, size, i, message;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int rBufer;
        fillVectors(n);
        start = 0;
        stop = start + n / size + n % size;
        for (int i = 1; i < size; i++) {
            start = stop;
            stop = std::min(start + n / size, n);
            MPI_Send(&array1[start], stop-start, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&array2[start], stop-start, MPI_INT, i, 2, MPI_COMM_WORLD);
        }
        start = 0;
        stop = start + n / size + n % size;
        for (int i = start; i < stop; i++) sBufer+= array1[i] * array2[i];
        cout << "Process " << rank << ": result = " << sBufer << endl;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&rBufer, n / size, MPI_INT, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, &status);
            sBufer += rBufer;
        }
        cout << "Final result = " << sBufer << endl;
    }
    else { 
        MPI_Recv(&array1, n / size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status); 
        MPI_Recv(&array2, n / size, MPI_INT, 0, 2, MPI_COMM_WORLD, &status); 
        for (int i = 0; i < n / size; i++) sBufer += array1[i] * array2[i]; 
        MPI_Send(&sBufer, 1, MPI_INT, 0, 3, MPI_COMM_WORLD); 
        cout << "Process " << rank << ": result = " << sBufer << endl;
    }
    MPI_Finalize(); 
    return 0; 
}