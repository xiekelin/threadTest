#include "ThreadPool.h"
#include "Worker.h"
#include "TestThreadPool.h"


namespace WBAIPayment
{
int ThreadPool::initPool(int threadNum)
{
	maxPoolSize = threadNum;
	minPoolSize = 1;
	changeSize = 1;
	timeIndex = 0;
	totalSleepTime = 0;
	keepAliveTime = 3;
	// chrono::high_resolution_clock::time_point startTime;
	// chrono::high_resolution_clock::time_point endTime;
	// startTime =  chrono::high_resolution_clock::now();
	// for (int i = 0 ; i < maxPoolSize; i++)
	// {
	// 	shared_ptr<Worker> wk = make_shared<Worker>();
	// 	workerQueue.push(move(wk));
	// }
	// endTime = chrono::high_resolution_clock::now();
	// chrono::duration<double> tt =endTime-startTime;
	// cout << "consume time :" << tt.count() << endl;
	// startTime =  chrono::high_resolution_clock::now();
	// for (int i = 0 ; i < maxPoolSize; i++)
	// {
	// 	workerQueue.pop();
	// }
	// endTime = chrono::high_resolution_clock::now();
	// tt = endTime - startTime ;
	// cout << "consume time :" << tt.count() << endl;
	distributeThreadStart();
}

void ThreadPool::distributeThreadStart()
{
	distributeThread = thread ( [&]()
	{
		gettimeofday(&start, NULL);
		while (!stop_thread)
		{
			if (runnableQueue.size() > 0 )
			{
				timeIndex = 0;
				totalSleepTime = 0;
				if (workerQueue.size() == 0 && poolSize < maxPoolSize)
				{

					//cout << "add thread" << endl;
					for (int i = 0; i < changeSize; i++)
					{
						if (poolSize >= maxPoolSize) break;
						shared_ptr<Worker> wk = make_shared<Worker>();
						lock_guard<mutex>lockGard(worker_mtx);
						//worker_mtx.lock();
						workerQueue.push(move(wk));
						//worker_mtx.unlock();
						poolSize = poolSize + 1;
					}
					//gettimeofday(&end, NULL);
					//cout<<"add thread consume time:"<< 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec<<endl;
				}
				if (workerQueue.size() > 0)
				{

					auto & tk = workerQueue.front();
					tk->setRunnable(runnableQueue.front());
					tk->setWorkerRef(tk);
					tk->waitFinishPushWk();
					lock_guard<mutex>lockGard1(runnable_mtx);
					//runnable_mtx.lock();
					runnableQueue.pop();
					//runnable_mtx.unlock();

					lock_guard<mutex>lockGard(worker_mtx);
					//worker_mtx.lock();
					workerQueue.pop();
					//worker_mtx.unlock();


				}
				else
				{
					unique_lock<mutex> lck(worker_mtx);
					m_condition.wait(lck, [&] {return workerQueue.size() != 0;});
				}
				continue;
			}
			else
			{
				// gettimeofday(&end, NULL);
				// cout << "get thread consume time:" << 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec << endl;
				// break;
				// if (poolSize > minPoolSize)
				// {
				// 	if (workerQueue.size() > 0)
				// 	{
				// 		for (int i = 0; i < changeSize; i++)
				// 		{
				// 			if (poolSize == 0 || poolSize == minPoolSize)break;
				// 			if (workerQueue.front()->getKeepAliveTime().count() > keepAliveTime)
				// 			{
				// 				cout<<"delete:"<<endl;
				// 				worker_mtx.lock();
				// 				workerQueue.pop();
				// 				worker_mtx.unlock();
				// 				poolSize = poolSize - 1;
				// 			}
				// 			else
				// 			{
				// 				// break;
				// 				unique_lock<mutex> lck(runnable_mtx);
				// 				m_condition.wait_for(lck, chrono::seconds(keepAliveTime));
				// 			}
				// 		}
				// 	}
				// 	else
				// 	{
				// 		int sleepTime = pow(2, timeIndex);
				// 		totalSleepTime = totalSleepTime + sleepTime;
				// 		timeIndex = timeIndex + 1;
				// 		unique_lock<mutex> lck(runnable_mtx);
				// 		cout << "delete------:" << sleepTime << endl;
				// 		m_condition.wait_for(lck, chrono::seconds(sleepTime));
				// 		if (totalSleepTime >= 60)
				// 		{
				// 			//没有业务处理逻辑函数时，阻塞线程
				// 			unique_lock<mutex> lck(runnable_mtx);
				// 			m_condition.wait(lck);
				// 		}
				// 	}
				// }
				// else
				// {
				// 	cout << "in block" << endl;
				// 	//没有业务处理逻辑函数时，阻塞线程
				// 	unique_lock<mutex> lck(runnable_mtx);
				// 	m_condition.wait(lck);
				// }
				if (isTest)
				{

					//cout << "workerQueue:" << workerQueue.size() << " poolSize:" << poolSize << endl;
					worker_mtx.lock();
					if (workerQueue.size() == poolSize)
					{
						worker_mtx.unlock();
						if (ttp->callBack())
						{
							continue;
						}
						else
							break;
					}
					worker_mtx.unlock();

					// usleep(1);
					// runnable_mtx.unlock();
				}
			}
		}
	});
}

void ThreadPool::pushTask(shared_ptr<Runnable> rb)
{
	unique_lock<mutex> lck(runnable_mtx);
	runnableQueue.push(move(rb));
	m_condition.notify_one();
}

void ThreadPool::pushWorkerQueue(shared_ptr<Worker> worker)
{
	unique_lock<mutex> lck(worker_mtx);
	workerQueue.push(move(worker));
	m_condition.notify_one();
}

void ThreadPool::stop()
{
	stop_thread  = true;
	while (poolSize)
	{
		if (workerQueue.size() > 0 )
		{
			workerQueue.pop();
			poolSize--;
		}
	}
}

void ThreadPool::set_testRef(TestThreadPool* g)
{
	ttp = g;
}

void ThreadPool::set_isTest(bool value)
{
	isTest = value;
}


bool ThreadPool::cleanUp()
{
	changeSize = 1;
	timeIndex = 1;
	totalSleepTime = 0;
	keepAliveTime = 120;
	for (int i = 0; i < poolSize; i++)
	{
		workerQueue.pop();
	}
	poolSize = 0;
	return true;
}

shared_ptr<ThreadPool> ThreadPool::t_instance = 0;
shared_ptr<ThreadPool> ThreadPool::getInstance()
{
	if (nullptr == t_instance)
	{
		t_instance = make_shared<ThreadPool>();
	}
	return t_instance;
}

}