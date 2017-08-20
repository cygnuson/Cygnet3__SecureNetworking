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

#define _DEBUGMASTERLOCK _DEBUG && 0

namespace cg {

template<typename Mutex, bool Ref>
inline void WaitTimedBasicLock<Mutex, Ref>::Lock()
{
	if (TryLock())
	{
		return;
	}
	else
	{
#if _DEBUGMASTERLOCK
		LogError(__FUNCSTR__, "TryLock Fail, will block.");
#endif
	}
	BeforeLock();
	m_lock.lock();
	AfterLock();
}

template<typename Mutex, bool Ref>
inline void WaitTimedBasicLock<Mutex, Ref>::Unlock()
{
	m_lock.unlock();
}

template<typename Mutex, bool Ref>
inline bool WaitTimedBasicLock<Mutex, Ref>::TryLock()
{
	return m_lock.try_lock();
}
template<typename Mutex, bool Ref>
inline ScopeLock<WaitTimedBasicLock<Mutex, Ref>>
WaitTimedBasicLock<Mutex, Ref>::LockGuard()
{
	return ScopeLock<WaitTimedBasicLock<Mutex, Ref>> (*this,
		&WaitTimedBasicLock<Mutex, Ref>::Unlock,
		&WaitTimedBasicLock<Mutex, Ref>::Lock);
}

template<typename Mutex, bool Ref>
template<typename T>
inline ScopeLock<WaitTimedBasicLock<Mutex, Ref>, T>
WaitTimedBasicLock<Mutex, Ref>::LockGuard(
	T * member)
{
	return ScopeLock<WaitTimedBasicLock<Mutex, Ref>, T>(*this,
		&WaitTimedBasicLock<Mutex, Ref>::Unlock,
		&WaitTimedBasicLock<Mutex, Ref>::Lock, member);
}
/*****************************************************************************/
template<typename Mutex, bool Ref>
inline bool WaitTimedSharedLock<Mutex, Ref>::STryLock()
{
	return m_lock.try_lock_shared();
}
template<typename Mutex, bool Ref>
inline void WaitTimedSharedLock<Mutex, Ref>::SUnLock()
{
	m_lock.unlock_shared();
}
template<typename Mutex, bool Ref>
inline void WaitTimedSharedLock<Mutex, Ref>::SLock()
{
	if (STryLock())
	{
		return;
	}
	else
	{
#if _DEBUGMASTERLOCK
		LogError(1, __FUNCSTR__, "TryLock Fail, will block.");
#endif
	}
	BeforeLock();
	m_lock.lock_shared();
	AfterLock();
}
template<typename Mutex, bool Ref>
inline ScopeLock<WaitTimedSharedLock<Mutex, Ref>>
WaitTimedSharedLock<Mutex, Ref>::SLockGuard()
{
	return ScopeLock<WaitTimedSharedLock<Mutex, Ref>>(*this,
		&WaitTimedSharedLock<Mutex, Ref>::SUnLock,
		&WaitTimedSharedLock<Mutex, Ref>::SLock);
}
template<typename Mutex, bool Ref>
template<typename T>
inline ScopeLock<WaitTimedSharedLock<Mutex, Ref>, T>
WaitTimedSharedLock<Mutex, Ref>::SLockGuard(
	T * member)
{
	return ScopeLock<WaitTimedSharedLock<Mutex, Ref>, T>(*this,
		&WaitTimedSharedLock<Mutex, Ref>::SUnLock,
		&WaitTimedSharedLock<Mutex, Ref>::SLock, member);
}
/*****************************************************************************/

template<typename Mutex, bool Ref>
template<typename T>
inline bool WaitTimedTimedLock<Mutex, Ref>::
TryLockUntil(const T & chronoTimePoint)
{
	return m_lock.try_lock_until(chronoTimePoint);
}
template<typename Mutex, bool Ref>
template<typename T>
inline bool WaitTimedTimedLock<Mutex, Ref>::
TryLockFor(const T & chronoDuration)
{
	return m_lock.try_lock_for(chronoDuration);
}
/*****************************************************************************/

template<typename Mutex, bool Ref>
template<typename T>
inline bool WaitTimedSharedTimedLock<Mutex, Ref>::
STryLockUntil(const T & chronoTimePoint)
{
	return m_lock.try_lock_until_shared(chronoTimePoint);
}
template<typename Mutex, bool Ref>
template<typename T>
inline bool WaitTimedSharedTimedLock<Mutex, Ref>::
STryLockFor(const T & chronoDuration)
{
	return m_lock.try_lock_for_shared(chronoDuration);
}
/*****************************************************************************/
template<typename Mutex, bool Ref>
template<typename TimeUnit>
inline TimeUnit WaitTimedLockMember<Mutex, Ref>::
TimeSpentSleeping() const
{
	return std::chrono::duration_cast<TimeUnit>(m_totalWaitTime);
}
template<typename Mutex, bool Ref>
inline std::chrono::nanoseconds WaitTimedLockMember<Mutex, Ref>::ResetTimer()
{
	auto copy = m_totalWaitTime;
	m_totalWaitTime = std::chrono::nanoseconds(0);
	return copy;
}

template<typename Mutex, bool Ref>
inline void WaitTimedLockMember<Mutex, Ref>::BeforeLock()
{
	m_lastStart = std::chrono::high_resolution_clock::now();
}

template<typename Mutex, bool Ref>
inline void WaitTimedLockMember<Mutex, Ref>::AfterLock()
{
#if _DEBUGMASTERLOCK
	auto callTime
		= (std::chrono::high_resolution_clock::now() - m_lastStart);
	m_totalWaitTime += callTime;
	auto seconds = std::chrono::duration_cast<std::chrono::microseconds>
		(callTime).count();
	LogWarn(__FUNCSTR__, "Blocked for ", seconds, " microseconds.");
#endif
}

template<typename Mutex>
template<typename TimeUnit>
inline TimeUnit WaitTimedLockMember<Mutex, true>::
TimeSpentSleeping() const
{
	return std::chrono::duration_cast<TimeUnit>(m_totalWaitTime);
}
template<typename Mutex>
inline std::chrono::nanoseconds WaitTimedLockMember<Mutex, true>::ResetTimer()
{
	auto copy = m_totalWaitTime;
	m_totalWaitTime = std::chrono::nanoseconds(0);
	return copy;
}

template<typename Mutex>
inline void WaitTimedLockMember<Mutex, true>::BeforeLock()
{
	m_lastStart = std::chrono::high_resolution_clock::now();
}

template<typename Mutex>
inline void WaitTimedLockMember<Mutex, true>::AfterLock()
{
#if _DEBUGMASTERLOCK
	auto callTime
		= (std::chrono::high_resolution_clock::now() - m_lastStart);
	m_totalWaitTime += callTime;
	auto seconds = std::chrono::duration_cast<std::chrono::microseconds>
		(callTime).count();
	LogWarn(1, __FUNCSTR__, "Blocked for ", seconds, " microseconds.");
#endif
}

}