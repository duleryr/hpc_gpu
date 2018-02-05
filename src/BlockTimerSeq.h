/*
	This file manages time measures for sequential algorithms
*/

#ifndef DEF_BLOCK_TIMER_SEQ
#define DEF_BLOCK_TIMER_SEQ

#include <iostream>
#include <ctime>

class BlockTimerSeq
{
protected:
	clock_t beginTime;

public:
	BlockTimerSeq() : beginTime(clock())
	{
	}

	void display(std::ostream &os, const char* timerNameShort, const char* timerNameLong, int numThreads)
	{
		const float cpuTime = (float)(clock() - beginTime) / (float)CLOCKS_PER_SEC;
		os << timerNameShort << " :: " << timerNameLong << " :" << std::endl
			<< "\tTime elapsed :: " << cpuTime << "s :: " << " using " << numThreads << " threads;" << std::endl << std::endl;
	}

	void reset()
	{
		this->beginTime = clock();
	}
};


#endif
