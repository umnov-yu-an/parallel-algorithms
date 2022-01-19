#include <iostream>
#include "mpi.h"
using namespace std;
int main(int argc, char **argv)
{
	int rank;
	MPI_Init(&argc, &argv);
	MPI_File fh;
	char* buf = new char[12];
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_File_open(MPI_COMM_WORLD, "Bulk_Text.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	MPI_File_set_view(fh, rank * 12, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
	MPI_File_read_all(fh, buf, 12, MPI_CHAR, MPI_STATUS_IGNORE);
	cout << "Process " << rank << " reads " << buf << endl;
	MPI_File_close(&fh);
	MPI_Finalize();
}