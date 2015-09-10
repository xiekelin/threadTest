#include <atomic>

namespace WBAIPayment 
{
class SimpleSpinLock
{
	std::atomic_flag lockStatus = ATOMIC_FLAG_INIT;
public:
	SimpleSpinLock() = default;
	SimpleSpinLock(const SimpleSpinLock &other) = delete;
	SimpleSpinLock& operator=(SimpleSpinLock&) = delete;
	void lock();
        void unlock();
};
}
