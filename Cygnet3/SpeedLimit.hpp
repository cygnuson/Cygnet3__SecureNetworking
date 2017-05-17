#pragma once

#include <thread>
#include <cmath>
#include <chrono>

namespace cg {

/**Class for limiting the rate that a program can run run.

The speed limit will block with operator() untill a certian amount of time has
passed sense the last call to operator().
*/
class SpeedLimit
{
public:
	/**The high resolution clock.*/
	using HRClock = std::chrono::high_resolution_clock;
	/**The time point to use.*/
	using TPoint = HRClock::time_point;
	/**The type of duration to keep track with.*/
	using DurationType = std::chrono::nanoseconds;
	/**Create a speed limiter
	\param t The minimum time between calls to oeprator()*/
	SpeedLimit(DurationType t);
	/**Create a speed limiter with not set speed.*/
	SpeedLimit();
	/**Copy the limiter.
	\param other The other limiter to use.*/
	SpeedLimit(const SpeedLimit& other);
	/**Copy the limiter.
	\param other The other limiter to use.*/
	void operator=(const SpeedLimit& other);
	/**Create a speed limiter with a set frame rate in frames per sec.
	\param frameRate The FPS that is needed.*/
	SpeedLimit(double frameRate);
	/**Set the speed limiter to cause minium frame rate.*/
	void SetFPS(double frameRate);
	/**Change the minimum amount of time between calls to operator()
	\param t The amount of time in std::chrono::duration*/
	void SetTime(DurationType t);
	/**Activate the speed limit. If Time() in units of T (the template param)
	has not passed, the operator() will block untill that proper amount of time
	has passed.*/
	void operator()();
	/**Increase the amount of time that is required to pass between calls to
	operator().
	\param amt The amount to increase.*/
	void operator += (DurationType amt);
	/**Decrease the amount of time that is required to pass between calls to
	operator().
	\param amt The amount to decrease.*/
	void operator -=(DurationType amt);
	/**Set the diabled status.
	\param set The value of the DISABLED flag.  If true, the limiter will be
	disabled.*/
	void Disable(bool set);
	/**Determine if the limiter is diabled.
	\return True if the limiter is disabled.*/
	bool IsDisabled() const;
private:
	/**delete the Move ctor.*/
	SpeedLimit(SpeedLimit&&) = delete;
	/**delete the Move ctor.*/
	void operator=(SpeedLimit&&) = delete;
	/**The last time theat the operator() succeeded.*/
	TPoint m_lastTick;
	/**The target frame rate.*/
	double m_expectedFrameRate = 0.0f;
	/**disable flag. if true,no blocking will happen.*/
	bool m_disabled = false;
	/**The time to wait between blocks.*/
	DurationType m_time;
};


}
