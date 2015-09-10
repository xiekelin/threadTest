#ifndef _SERVICE_NEWTHREADPOOLT_H_
#define _SERVICE_NEWTHREADPOOLT_H_

#include <thread>
#include <vector>
#include <mutex>
#include "Runnable.h"
#include <map>
#include <condition_variable>
#include <iostream>
#include <unistd.h>
#include  <functional>
#include <queue>
#include <time.h>
#include <sys/time.h>
#include "SimpleSpinLock.h"
#include <memory.h>
#include <chrono>
#include <atomic>
#include <cmath>

using namespace std;

namespace WBAIPayment
{

class Worker;

class TestThreadPool;

class ThreadPool
{
public:
	ThreadPool(): stop_thread(false) {};

	~ThreadPool() {
	};

	int initPool (int threadNum);

	void pushTask(shared_ptr<Runnable> rb);

	/*
	* 工作线程工作完之后重新入队列；
	*/
	void pushWorkerQueue(shared_ptr<Worker> worker);

	void stop();

	void set_testRef(TestThreadPool* g);

	void set_isTest(bool value);

	bool cleanUp();

	void setMaxPoolSize(int maxPoolSize)
	{
		this->maxPoolSize = maxPoolSize;
	}

	void setMinPoolSize(int minPoolSize)
	{
		this->minPoolSize = minPoolSize;
	}

	void setChangeSize(int changeSize)
	{
		this->changeSize = changeSize;
	}

	void setKeepAliveTime(int keepAliveTime)
	{
		this->keepAliveTime = keepAliveTime;
	}
	static  shared_ptr<ThreadPool> getInstance();

private:

	mutex runnable_mtx;

	mutex worker_mtx;

	long poolSize;

	//表示线程是否停止
	bool stop_thread;

	//分发线程引用，保护线程不被析构
	thread distributeThread;

	//
	thread deleteWorkerThread;

	condition_variable m_condition;

	condition_variable test_condition;

	queue<shared_ptr<Runnable>> runnableQueue;

	queue<shared_ptr<Worker>> workerQueue;

	static shared_ptr<ThreadPool>  t_instance;

	SimpleSpinLock ssLock;

	bool isTest{false};

	TestThreadPool * ttp;

	chrono::high_resolution_clock::time_point startTime;

	void distributeThreadStart();


	int maxPoolSize;

	int minPoolSize;

	int changeSize;

	int keepAliveTime;

	int timeIndex;

	int totalSleepTime;
	struct timeval start;
	struct timeval end;

} ;
}
#endif
