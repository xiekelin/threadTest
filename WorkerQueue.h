#ifndef _SERVICE_WORKERQUEUE_H_
#define _SERVICE_WORKERQUEUE_H_

#include <vector>
#include <mutex>
#include <memory.h>
#include <chrono>
#include "Worker.h"
using namespace std;

namespace WBAIPayment
{

class WorkerQueue
{

	struct Comparator
	{
		bool operator()( const shared_ptr<Worker> &one, const  shared_ptr<Worker> &other)
		{
			return (one->getKeepAliveTime() - other->getKeepAliveTime()).count() < 0; //最大值
		}
	};
public:

	priority_queue<shared_ptr<Worker>, vector<shared_ptr<Worker>>, Comparator> & getQueue()
	{
		return workerQueue;
	}
private:
	priority_queue<shared_ptr<Worker>, vector<shared_ptr<Worker>>, Comparator> workerQueue;
	queue<shared_ptr<Runnable>> workerQueue;


} ;
}
#endif
