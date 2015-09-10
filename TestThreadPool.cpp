#include "TestThreadPool.h"
#include "ThreadPool.h"
namespace WBAIPayment
{
void TestThreadPool::init(int threadNum, int testSum)
{
	tp = ThreadPool::getInstance();
	for (int i = 0; i < testSum; i++)
	{
		shared_ptr<kass> k1 = make_shared<kass>();
		tp->pushTask(k1);
	}
	tp->set_testRef(this);
	tp->set_isTest(true);
	timer = 0;
	gettimeofday(&start, NULL);
	tp->initPool(threadNum);
}
bool TestThreadPool::callBack()
{
	gettimeofday(&end, NULL);
	if (runCount != 0)
	{
		timer = timer + 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec ;
		cout <<  " time: " << timer << " count:" << runCount << endl;
	}
	runCount = runCount + 1;
	if (runCount != 6)
	{
		for (int i = 0; i < testSum; i++)
		{
			shared_ptr<kass> k1 = make_shared<kass>();
			tp->pushTask(k1);
		}
		if (tp->cleanUp())
		{
			gettimeofday(&start, NULL);
			return true;
		}
		// gettimeofday(&start, NULL);
		// return true;
	}
	else
	{
		double avgTimer = (double)timer / 5000000;
		cout << "finish test . the avgTimer is :" << (double)timer / 5000000 << "  and deal logic fun  sum is :" << (double)testSum / avgTimer << endl;
		return false;
	}
}
}