#include <iostream>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING = 100;

using namespace std;

int main(void) {
	char greeting[MAX_STRING];
	int comm_sz;
	int my_rank;
	int sum = 0;
	int recv_sum = 0;


	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	int chunkSize = 10000 / comm_sz;
	int lower = my_rank * chunkSize;
	int upper = lower + chunkSize;

	for (int i = lower + 1; i <= upper; i++) {
		sum = sum + i;
	}

	if (my_rank != 0) {
		MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	} else {
		printf("sum from process %d = %d\n", my_rank, sum);
		for (int q = 1; q < comm_sz; q++) {
			MPI_Recv(&recv_sum, 1, MPI_INT, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("sum from process %d = %d\n", q, recv_sum);
			sum = sum + recv_sum;
		}
		printf("TOTAL SUM: %d", sum);
	}

	MPI_Finalize();

	return 0;
}