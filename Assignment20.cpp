#include <iostream>
#include "mpi.h"
#define BUFSIZE 100
using namespace std;

void createTxt(const char* fname, const char* text, int length)
{
	MPI_File fh;
	MPI_Status status;
	MPI_File_open(MPI_COMM_WORLD, fname, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
	MPI_File_write(fh, text, length, MPI_CHAR, &status);
	MPI_File_close(&fh);
}

int main(int argc, char **argv)
{
	int bufsize, num, sum;
	MPI_Init(&argc, &argv);
	MPI_Status status;
	createTxt("Text.txt", "I'm text, READ ME!", 18);
	MPI_File fh;
	char buf[BUFSIZE];
	MPI_File_open(MPI_COMM_WORLD, "Text.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	MPI_File_set_view(fh, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
	sum = 0;
	do {
		MPI_File_read(fh, buf, BUFSIZE, MPI_CHAR, &status);
		MPI_Get_count(&status, MPI_CHAR, &num);
		cout << "buf = " << buf << endl;
		sum += num;
	} while (num >= BUFSIZE);
	MPI_File_close(&fh);
	MPI_File_delete("Text.txt", MPI_INFO_NULL);
	cout << "Total number of symbols in the text = " << sum << endl;
	MPI_Finalize();
}