// std_tr1__memory__dynamic_pointer_cast.cpp
// compile with: /EHsc
#include <memory>
#include <iostream>
#include "ThreadPool.h"
#include "Runnable.h"
#include <sys/time.h>
  #include "TestThreadPool.h"

#include<vector>
using namespace std;
using namespace WBAIPayment;

class kass : public Runnable
{
public:
    virtual void run()
    {
        //cout << "thread " << this_thread::get_id() << "working :" << key << endl;
        struct  timeval  start;
          unsigned long timer;
        gettimeofday(&start, NULL);
        while (true)
        {
            timeval end;
            gettimeofday(&end, NULL);
            timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
            if (timer > 1 * 1000)
            {
                break;
            }
        }
     //   cout<<"runnable time:"<<timer<<endl;
    };
    int execute(void* arg)
    {
        //具体业务逻辑
        return 0;
    };
    int key;
};


int main()
{
    TestThreadPool t(8,100000);
    // for (int i = 0; i < 100000; i++)
    // {
    //     shared_ptr<kass> r = make_shared<kass>();
    //     ThreadPool::getInstance()->pushTask(r);
    // }
    // ThreadPool::getInstance()->initPool(15);
    // sleep(5);
    // for (int i = 0; i < 50; i++)
    // {
    //     cout<<"add"<<endl;
    //     shared_ptr<kass> r = make_shared<kass>();
    //     ThreadPool::getInstance()->pushTask(r);
    // }
    sleep(111111111);
    return (0);
}
