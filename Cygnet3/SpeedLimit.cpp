#include "SpeedLimit.hpp"

namespace cg {


cg::SpeedLimit::SpeedLimit(DurationType t)
{
	SetTime(t);
}

SpeedLimit::SpeedLimit()
{
	m_time = std::chrono::seconds(1);
}

SpeedLimit::SpeedLimit(const SpeedLimit & other)
	:m_lastTick(other.m_lastTick),
	m_expectedFrameRate(other.m_expectedFrameRate),
	m_disabled(other.m_disabled),
	m_time(other.m_time)
{

}

void SpeedLimit::operator=(const SpeedLimit & other)
{
	m_lastTick = other.m_lastTick;
	m_expectedFrameRate = other.m_expectedFrameRate;
	m_disabled = other.m_disabled;
	m_time = other.m_time;
}

SpeedLimit::SpeedLimit(double frameRate)
{
	SetFPS(frameRate);
}

void SpeedLimit::SetFPS(double frameRate)
{
	if (frameRate != 0)
	{
		m_expectedFrameRate = frameRate;
		double time = double(1) / m_expectedFrameRate;
		time *= 1e9;
		SetTime(DurationType((uint64_t)std::floor(time)));
	}
	else
	{
		m_time = std::chrono::seconds(1);
		m_disabled = true;
	}
}

void SpeedLimit::operator()()
{
	if (m_disabled)
		return;

	auto spent = (HRClock::now() - m_lastTick);
	if (spent < m_time)
	{
		auto sleepTime = m_time - spent;
		std::this_thread::sleep_for(sleepTime);
	}
	m_lastTick = HRClock::now();
}

void SpeedLimit::Disable(bool set)
{
	m_disabled = set;
}

bool SpeedLimit::IsDisabled() const
{
	return m_disabled;
}

void SpeedLimit::SetTime(DurationType t)
{
	m_time = t;
	m_lastTick = HRClock::now();
	m_disabled = false;
}

void SpeedLimit::operator+=(DurationType amt)
{
	m_time += amt;
}

void SpeedLimit::operator-=(DurationType amt)
{
	m_time -= amt;
}

}