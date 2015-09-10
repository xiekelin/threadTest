#ifndef _SERVICE_WORKER_H_
#define _SERVICE_WORKER_H_

#include <mutex>
#include <condition_variable>
#include  <iostream>
#include <thread>
#include <vector>
#include "Runnable.h"
#include  <functional>
#include <sys/time.h>
#include <chrono>

using namespace std;

namespace WBAIPayment
{
class ThreadPool;
class Worker
{
public:
	Worker(): wk(nullptr), stop_thread(false)
	{
		//初始化线程
		startTime = chrono::high_resolution_clock::now();
		workerThread = thread(&Worker::run, this);
	};
	~Worker()
	{
		//cout << "this is  xigou function:"<<(wk==nullptr) << endl;
		wk.reset();
		stop_thread = true;
		m_condition.notify_all();
		workerThread.join();
		m_condition.notify_all();//?
	}

	void setRunnable(shared_ptr<Runnable> runnable);

	/*
	* 设置自身引用，用以重新入分发线程的worker队列
	*/
	bool setWorkerRef(shared_ptr<Worker> wkt);

	void waitFinishPushWk();

	void stop();

	chrono::duration<double> getKeepAliveTime()
	{
		return chrono::high_resolution_clock::now() - startTime;
	}

private:
	//用于worker线程阻塞的锁
	mutex worker_mtx;

	condition_variable m_condition;

	//自身引用，保护线程不被析构
	shared_ptr<Worker> wk;

	thread workerThread;

	shared_ptr<Runnable> _runnable ;

	//表示线程是否停止
	bool  stop_thread;

	chrono::high_resolution_clock::time_point startTime;

	void run();

	struct timeval start;
	struct timeval end;
};
}
#endif