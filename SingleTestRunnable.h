#ifndef _SERVICE_SINGTESTRUNNABLE_H_
#define _SERVICE_SINGTESTRUNNABLE_H_

#include "Runnable.h"
#include <queue>
#include <thread>
#include <mutex>
#include <memory.h>
#include <sys/time.h>
#include <iostream>
#include <condition_variable>
#include <unistd.h>
using namespace std;

namespace WBAIPayment
{
class tt
{
public:
	~tt()
	{
		cout << "xigou" << endl;
	}
};
class SingleTestRunnable
{
public:
	SingleTestRunnable(int num , int threadNum)
	{
		this->runableNum = num;
		init(num, threadNum);
	}
	void test_start()
	{
		cout << "start the SingleTestRunnable:" << this_thread::get_id() << endl;
		gettimeofday(&start, NULL);
		m_condition.notify_all();

	}
private:
	queue<shared_ptr<Runnable>> runnableQueue;

	queue<thread> threadQueue;

	SimpleSpinLock sslock;

	bool stop_thread{false};

	mutex mtx;
	mutex kmtx;
	condition_variable m_condition;

	struct timeval start, end;
	int count = 5;
	long int timer = 0;

	int runableNum;

	void run()
	{
		unique_lock<mutex> lck(mtx);
		m_condition.wait(lck);
		lck.unlock();
		while (!stop_thread)
		{
			//lock_guard<SimpleSpinLock> lockGard(sslock);
			//cout<<"the size is:"<<runnableQueue.size()<<"thread id is :"<<this_thread::get_id()<<endl;
			kmtx.lock();
			if (runnableQueue.size() > 0)
			{
				kmtx.unlock();
				runnableQueue.front()->run();
				lock_guard<mutex>lockGard(kmtx);
				runnableQueue.pop();
			}
			else
			{	
				gettimeofday(&end, NULL);
				 timer = timer+1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
				cout <<  " time: " << timer << " count:" << count << "     id :"  << this_thread::get_id() << endl;
				count = count - 1;
				for (int i = 0; i < runableNum; i++)
				{
					shared_ptr<Runnable> k1 = make_shared<Runnable>();
					runnableQueue.push(move(k1));
				}
				if (count <= 0)
				{
					double avgTimer = (double)timer / 5000000;
					cout << "finish test . the avgTimer is :" << (double)timer / 5000000 << "  and deal logic fun  sum is :" << (double)runableNum / avgTimer << endl;
					stop_thread = true;
				}
				gettimeofday(&start, NULL);
				//stop_thread = true;
				kmtx.unlock();
			}
		}
	}
	void init(int num , int threadNum)
	{
		for (int i = 0; i < num; i++)
		{
			shared_ptr<Runnable> k1 = make_shared<Runnable>();
			runnableQueue.push(move(k1));
		}
		for (int i = 0; i < threadNum ; i++)
		{
			thread th(&SingleTestRunnable::run, this);
			threadQueue.push(move(th));
		}
	}
};

}

#endif