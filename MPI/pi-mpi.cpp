// ConsoleApplication2.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "mpi.h"  
#include <stdio.h>  
#include <Windows.h>
#define NUM_STEPS 1000

int main(int argc, char* argv[])
{
	int nprocs;
	int myid;
	int i;
	double x, pi;
	double sum = 0.0;
	double step = 1.0 / (double)NUM_STEPS;

	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBeginTime);

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	for (i = myid; i < NUM_STEPS; i += nprocs) {
		x = (i + 0.5) * step;
		sum = sum + 4.0 / (1.0 + x * x);
	}
	sum = step * sum;
	MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (myid == 0){
		printf("parallel program results with %d processes:\n", nprocs);
		printf("pi = %g  (%17.15f)\n", pi, pi);
		QueryPerformanceCounter(&nEndTime);
		printf("time = %lfms\n", (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart * 1000);
	}

	MPI_Finalize();

	
	return 0;
}
