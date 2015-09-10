#ifndef SERVER_TESTTHREADPOOL_H
#define SERVER_TESTTHREADPOOL_H

#include "Runnable.h"
#include <chrono>
#include <iostream>
#include <sys/time.h>
#include  <memory>
using namespace std;

namespace WBAIPayment
{
class ThreadPool;
class kass : public Runnable
{
public:
	virtual void run()
	{
		//cout<<"working "<<endl;
		struct  timeval  start;
		gettimeofday(&start, NULL);
		while (true)
		{
			timeval end;
			gettimeofday(&end, NULL);
			unsigned long timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
			if (timer > 1 * 100000)
			{
				break;
			}
		}
	};
	int execute(void* arg)
	{
		//具体业务逻辑
		return 0;
	};
};

class TestThreadPool
{
public:
	TestThreadPool(int threadNum,int testSum)
	{
		this->threadNum = threadNum;
		this->testSum = testSum;
		init(threadNum,testSum);
	}
	void init(int threadNum,int testSum);

	bool callBack();
private:
	shared_ptr<ThreadPool> tp ;
	struct timeval start;
	struct timeval end;
	long int timer;
	int runCount{1};
	int threadNum;
	int testSum;
};
}

#endif