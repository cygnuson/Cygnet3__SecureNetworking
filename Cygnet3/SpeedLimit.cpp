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