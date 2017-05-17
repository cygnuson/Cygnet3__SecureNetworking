#include "Timer.hpp"

namespace cg {

Timer::Timer()
{

}

Timer::Timer(const Timer & other)
	:m_running(other.m_running)
{
	std::copy(other.m_starts.begin(), other.m_starts.end(), m_starts.begin());
	std::copy(other.m_stops.begin(), other.m_stops.end(), m_stops.begin());
}

Timer::Timer(Timer && other)
	:m_running(other.m_running)
{
	m_starts = std::move(other.m_starts);
	m_stops = std::move(other.m_stops);
}

void Timer::operator=(const Timer & other)
{
	m_running = other.m_running;
	std::copy(other.m_starts.begin(), other.m_starts.end(), m_starts.begin());
	std::copy(other.m_stops.begin(), other.m_stops.end(), m_stops.begin());
}

void Timer::operator=(Timer && other)
{
	m_running = other.m_running;
	m_starts = std::move(other.m_starts);
	m_stops = std::move(other.m_stops);
}

void Timer::Start()
{
	m_lock.lock_shared();
	m_starts.push_back(HRClock::now());
	++m_running;
}

void Timer::Stop()
{
	if (m_running == 0)
		throw cg::ex::ExNotRunning();
	m_stops.push_back(HRClock::now());
	--m_running;
	m_lock.unlock_shared();
	if (m_running == 0)
	{
		if (m_lock.try_lock()) {
			mcv_waiter.notify_all();
			m_lock.unlock();
		}
	}
}

void Timer::Clear()
{
	m_lock.lock();
	m_starts.clear();
	m_stops.clear();
	m_running = 0;
	m_lock.unlock();
}

std::chrono::nanoseconds Timer::Get()
{
	if (m_running > 0)
		throw cg::ex::ExStillRunning();
	m_lock.lock();
	std::chrono::nanoseconds elap = std::chrono::nanoseconds(0);
	/**If m_running == 0, then the list sizes must be equal.*/
	auto size = m_starts.size();
	for (std::size_t i = 0; i < size; ++i)
	{
		elap += (m_stops[i] - m_starts[i]);
	}
	m_lock.unlock();
	return elap;
}

uint64_t Timer::GetNano()
{
	return Get().count();
}

uint64_t Timer::GetMicro()
{
	return
		std::chrono::duration_cast<std::chrono::microseconds>(Get()).count();
}

uint64_t Timer::GetMilli()
{
	return
		std::chrono::duration_cast<std::chrono::milliseconds>(Get()).count();
}

double Timer::GetSec()
{
	return
		std::chrono::duration_cast
		<std::chrono::duration<double>>(Get()).count();
}

double Timer::GetMin()
{
	return std::chrono::duration_cast<
		std::chrono::duration<double, std::ratio<60>>>(Get()).count();
}


}