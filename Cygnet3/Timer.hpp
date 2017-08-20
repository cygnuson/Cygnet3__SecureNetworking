/*

(C) Matthew Swanson

This file is part of Cygnet3.

Cygnet3 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

Cygnet3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Cygnet3.  If not, see <http://www.gnu.org/licenses/>.

*/
#pragma once

#include <vector>
#include <shared_mutex>
#include <condition_variable>

#include "ChronoString.hpp"
#include "SpeedLimit.hpp"
#include "exception.hpp"
#include "Waitable.hpp"

namespace cg {

/**Ad object for keeping track of the passage of time.  Allows multiple starts
and stop in a stopwatch configuration.*/
class Timer : public cg::Waitable
{
public:
	/**The high resolution clock.*/
	using HRClock = std::chrono::high_resolution_clock;
	/**The time point to use.*/
	using TPoint = HRClock::time_point;
	/**Create the timer*/
	Timer();
	/**Copy a timer, useful for braching off while being timed.
	\param other The other timer to copy.*/
	Timer(const Timer& other);
	/**Move this timer to another spot.
	\param other The other timer to move to.*/
	Timer(Timer&& other);
	/**Set this equal to a different timer (copy).
	\param other The other timer.*/
	void operator=(const Timer& other);
	/**Set this equal to a different timer (move).
	\param other The other timer.*/
	void operator=(Timer&& other);
	/**Start the timer.*/
	void Start();
	/**Stop the timer.*/
	void Stop();
	/**Clear all recored times.*/
	void Clear();
	/**Get the current elapsed time.
	\return The time that the timer was running.
	\throws ExStillRunning if the timer is still running.*/
	std::chrono::nanoseconds Get();
	/**Get the current elapsed time.
	\return The time that the timer was running.
	\throws ExStillRunning if the timer is still running.*/
	uint64_t GetNano();
	/**Get the current elapsed time.
	\return The time that the timer was running.
	\throws ExStillRunning if the timer is still running.*/
	uint64_t GetMicro();
	/**Get the current elapsed time.
	\return The time that the timer was running.
	\throws ExStillRunning if the timer is still running.*/
	uint64_t GetMilli();
	/**Get the current elapsed time.
	\return The time that the timer was running.
	\throws ExStillRunning if the timer is still running.*/
	double GetSec();
	/**Get the current elapsed time.
	\return The time that the timer was running.
	\throws ExStillRunning if the timer is still running.*/
	double GetMin();
	/**Call a callable object FPS times per second.  The caller should return
	false when its time to stop running.
	\param fps the calls per second.  If FPS is zero, there will be no limit.
	\param caller The callable object to call FPS calls per sec.  The caller
	should return false when its time to stop running.
	\param amt The max amount of calls to make.
	\param args The arguments to call the caller with.*/
	template<typename Call, typename...Args>
	static void CallPerSec(double fps,
		uint64_t amt,
		Call && caller,
		Args&&...args);
	/**Call a callable object X amount of times over Y amount of durations.
	\param caller The callable object to call. when the caller returns false,
	the process stops.
	\param amt The amount of times to call the caller.
	\param duration The std::chrono::duration that it will take to call the
	caller amt times.
	\param args The args that go to the caller.*/
	template<typename Call, typename...Args>
	static void CallOverTime(uint64_t amt,
		std::chrono::nanoseconds duration,
		Call&& caller,
		Args&&...args);
	/**Execute a callable in a timed fasion.
	\param caller The callable to time.
	\param args The args to send to the caller.
	\tparam TimeUnit an std::chrono::duration that will be returned.
	\return The amount of time the execution took.*/
	template<typename TimeUnit = std::chrono::duration<float>,
		typename Caller, typename...Args>
		static TimeUnit TimedCall(Caller& caller,
			Args...args);
private:
	/**The start times of the timer.*/
	std::vector<TPoint> m_starts;
	/**the stop times of the timer.*/
	std::vector<TPoint> m_stops;
	/**The amount of intervals running.*/
	std::size_t m_running = 0;
	/**The mutex to make it thread safe.*/
	std::shared_timed_mutex m_lock;
};

template<typename Call, typename...Args>
inline void Timer::CallPerSec(double fps,
	uint64_t amt,
	Call && caller,
	Args&&...args)
{
	SpeedLimit sl;
	sl.SetFPS(fps);
	bool run = true;
	uint64_t calls = 0;
	while (run)
	{
		sl();
		run = caller(std::forward<Args>(args)...)
			&& ++calls <= (amt - 1);
	}
}

template<typename Call, typename ...Args>
inline void Timer::CallOverTime(uint64_t amt,
	std::chrono::nanoseconds duration,
	Call&& caller,
	Args&&...args)
{
	uint64_t nanoPerCall = duration.count() / amt;
	SpeedLimit sl((std::chrono::nanoseconds(nanoPerCall)));
	bool run = true;
	uint64_t calls = 0;
	while (run)
	{
		sl();
		run = caller(std::forward<Args>(args)...)
			&& ++calls <= (amt - 1);
	}
}

template<typename TimeUnit, typename Caller, typename ...Args>
inline TimeUnit Timer::TimedCall(Caller & caller,
	Args ...args)
{
	Timer timer;
	timer.Start();
	caller(std::forward<Args>(args)...);
    return std::chrono::duration_cast<TimeUnit>(timer.Get());
}

}
