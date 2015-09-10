#include "Worker.h"
#include "ThreadPool.h"

namespace WBAIPayment
{
void Worker::run()
{
	while (!stop_thread)
	{
		gettimeofday(&start, NULL);
		startTime = chrono::high_resolution_clock::now();
		if (_runnable != nullptr)
		{
			//接口类不为空，执行逻辑run函数
			_runnable->run();
			_runnable = nullptr;
			{
				unique_lock <mutex> lck(worker_mtx);
				ThreadPool::getInstance()->pushWorkerQueue(move(wk));
				m_condition.wait(lck);
			}
			gettimeofday(&end, NULL);
			//cout<<"run thread consume time:"<< 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec<<endl;
			continue;
		}
		unique_lock <mutex> lck(worker_mtx);
		m_condition.wait(lck);
	}
}

void Worker::setRunnable(shared_ptr<Runnable> runnable)
{
	_runnable = runnable;
}

void Worker::waitFinishPushWk()
{
	unique_lock <mutex> lck(worker_mtx);
	m_condition.notify_one();
}

bool Worker::setWorkerRef(shared_ptr<Worker> wkt)
{
	wk = wkt;
	return true;
}

void Worker::stop()
{
	stop_thread = true;
}

}