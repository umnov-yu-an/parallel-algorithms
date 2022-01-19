#include <iostream>
#include "mpi.h"
#define n 1000
using namespace std;

void smod5(void *a, void *b, int *l, MPI_Datatype *type) 
{
	int i;
	for (i = 0; i < *l; i++)
		((int*)b)[i] = (((int*)a)[i] + ((int*)b)[i]) % 5;
}

void findMax(void* fcomp, void* res, int* l, MPI_Datatype* dtp)
{
	for (int i = 0; i < *l; i++)
		((int*)res)[i] = max(((int*)fcomp)[i], ((int*)res)[i]);
}

int main(int argc, char **argv)
{
	int rank, size, i;
	int a[n], b[n], b_2[n];
	MPI_Init(&argc, &argv);
	MPI_Op op;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	for (i = 0; i < n; i++) a[i] = i + rank + 1;
	cout << "Process " << rank << " a[0] = " << a[0] << endl;

	//Max, using self-created function
	MPI_Op_create(&findMax, 1, &op);
	MPI_Reduce(a, b, n, MPI_INT, op, 0, MPI_COMM_WORLD);
	MPI_Op_free(&op);

	//Max, using built-in MPI_MAX 
	MPI_Reduce(a, b_2, n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		cout << "b[0] = " << b[0] << endl;
		int counter = 0, j;
		for (j = 0; j < n; j++) if (b[j] == b_2[j]) counter++;
		if (n == counter) cout << " Compared results are correct!" << endl;
		else cout << "Comrared results are incorrect!" << endl;
	}

	MPI_Finalize();
}