#include "Waitable.hpp"

namespace cg {

bool Waitable::WaitFor(std::chrono::nanoseconds to)
{
	if (ShouldStop())
		return true;
	std::mutex um;
	std::unique_lock<std::mutex> lock(um);
	if (to.count() < 0)
	{
		mcv_waiter.wait(lock, [&]() {
			return ShouldStop();
		});
		return true;
	}
	return mcv_waiter.wait_for(lock, to, [&]() {
		return ShouldStop();
	});
}

void Waitable::NotifyAll()
{
	OnNotify();
	mcv_waiter.notify_all();
}

void Waitable::NotifyOne()
{
	OnNotify();
	mcv_waiter.notify_one();
}


}