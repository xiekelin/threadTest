#ifndef _SERVICE_RUNNABLE_H_
#define _SERVICE_RUNNABLE_H_
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sys/time.h>
using namespace std;

namespace WBAIPayment
{
class Runnable
{
public:
	Runnable() {};

	virtual ~Runnable() {

	};

	virtual  void run() {
		struct  timeval  start;
		gettimeofday(&start, NULL);
		while (true)
		{
			timeval end;
			gettimeofday(&end, NULL);
			unsigned long timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
			if(timer > 1*1000)
			{
				break;
			}
		}
	};
	vector<map<string, string>>data;
};
}
#endif
