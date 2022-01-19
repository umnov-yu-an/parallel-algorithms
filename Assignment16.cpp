#include "mpi.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    int rank, new_rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm comm_revs;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 2; i < 4; i++) {
        MPI_Comm_split(MPI_COMM_WORLD, rank % i, size - rank, &comm_revs);
        MPI_Comm_rank(comm_revs, &new_rank);
        //Display rank è rank1
        cout << "(rank %% " << i << ") -> rank = " << rank << ", rank1 = " << new_rank << endl;
        MPI_Comm_free(&comm_revs);
    }
    MPI_Finalize();

    return 0;
}
