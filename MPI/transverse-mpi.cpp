// transverse-mpi.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "mpi.h"  
#include <stdio.h>
#include <cmath>
#include <Windows.h>

int main(int argc, char* argv[])
{
	int nprocs;
	int myid;
	int i, j;
	int aij;
	int temp;

	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;

	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBeginTime);

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	int NSTEPS = (int)sqrt(nprocs);
	MPI_Status sta;
	i = myid / NSTEPS;
	j = myid % NSTEPS;
	aij = myid;
	printf("(%d,%d):%d\n", i, j, aij);
	if (i < j) {
		MPI_Send(&aij, 1, MPI_INT, j*NSTEPS + i, 1, MPI_COMM_WORLD);
		MPI_Recv(&aij, 1, MPI_INT, j*NSTEPS + i, 2, MPI_COMM_WORLD, &sta);
	}
	else if (i > j) {
		temp = aij;
		MPI_Recv(&aij, 1, MPI_INT, j*NSTEPS + i, 1, MPI_COMM_WORLD, &sta);
		MPI_Send(&temp, 1, MPI_INT, j*NSTEPS + i, 2, MPI_COMM_WORLD);
	}
	if (myid == 0&&myid+1<nprocs) {
		QueryPerformanceCounter(&nEndTime);
		printf("time = %lfms\n", (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart * 1000);
		fprintf(stderr, "t(%d,%d):%d\n", i, j, aij);
		MPI_Send(&temp, 1, MPI_INT, myid+1, 3, MPI_COMM_WORLD);
	}
	else {
		MPI_Recv(&temp, 1, MPI_INT, myid - 1, 3, MPI_COMM_WORLD, &sta);
		fprintf(stderr, "t(%d,%d):%d\n", i, j, aij);
		if (myid + 1 < nprocs) {
			MPI_Send(&temp, 1, MPI_INT, myid + 1, 3, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();

	//QueryPerformanceCounter(&nEndTime);
	//printf("time = %lfms\n", (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart * 1000);
	return 0;
}


