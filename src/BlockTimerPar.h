/*
	This file manages time measures for parallel algorithms
*/

#ifndef DEF_BLOCK_TIMER_PAR
#define DEF_BLOCK_TIMER_PAR

#include <omp.h>
#include <iostream>

class BlockTimerPar
{
protected:
	double beginTime;

public:
	BlockTimerPar() : beginTime(omp_get_wtime())
	{
	}

	void display(std::ostream &os, const char* timerNameShort, const char* timerNameLong, int numThreads)
	{
		const double cpuTime = omp_get_wtime() - beginTime;
		os << timerNameShort << " :: " << timerNameLong << " :" << std::endl
			<< "\tTime elapsed :: " << cpuTime << "s :: " << " using " << numThreads << " threads;" << std::endl << std::endl;
	}

	void reset() {
		beginTime = omp_get_wtime();
	}
};

#endif
