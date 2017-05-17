#pragma once

#include <mutex>
#include <condition_variable>
#include <chrono>

namespace cg {

/**An object that can be waited on.*/
class Waitable
{
public:
	/**Try to force stop the waiting.*/
	virtual void StopWaiting() {
		mcv_waiter.notify_all();
	}
	/**Wait untill the object is done with whatever it is doing.*/
	virtual void Wait() {
		WaitFor(std::chrono::nanoseconds(-1));
	}
	/**Wait for an object to finish, but return false if it did not after
	`to` units of time pass. The units of time will be a chrono duration of 
	nanoseconds, but can be any duration type that will be cast automaticaly.
	if the timeout is 0, it will return instantly.  If the amount is less than
	zero, it will wait forever.
	\return True if the object finished.*/
	virtual bool WaitFor(std::chrono::nanoseconds to);
	/**Determine if stopping should happen.
	\return False to continue waiting.*/
	virtual bool ShouldStop() const {
		return false;
	}
	/**The function to run alongside the notify functions.*/
	virtual void OnNotify() {}
	/**Notify the object that waiting is done.*/
	virtual void NotifyAll();
	/**Notify the object that waiting is done.*/
	virtual void NotifyOne();
protected:
	/**A waiting variable to wait for something.*/
	std::condition_variable mcv_waiter;

};

}