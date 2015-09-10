#include "SimpleSpinLock.h"

using namespace WBAIPayment;

void SimpleSpinLock::lock()
{
	while(lockStatus.test_and_set(std::memory_order_acquire))
	;	
}

void SimpleSpinLock::unlock()
{
	lockStatus.clear(std::memory_order_release);
}
