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
#include "ScopeLock.hpp"


namespace cg {

template<typename Mutex, typename UseMember>
inline ScopeLock<Mutex, UseMember>::ScopeLock(Mutex & lock,
	FPtr parameter,
	FPtr lockFunc,
	UseMember * ref)
	:m_lock(lock), m_ptr(parameter)
{
	m_ref = ref;
	(m_lock.*lockFunc)();
}
template<typename Mutex, typename UseMember>
inline ScopeLock<Mutex, UseMember>::ScopeLock(
	ScopeLock<Mutex, UseMember>&& other)
	:m_ref(other.m_ref),
	m_lock(other.m_lock),
	m_ptr(other.m_ptr)
{
	/**invalidates the other scope lock.*/
	other.m_ref = nullptr;
}
template<typename Mutex, typename UseMember>
inline ScopeLock<Mutex, UseMember>::~ScopeLock()
{
	if (m_isUnlocked)
	{

	}
	else
	{
		(m_lock.*m_ptr)();
	}
}
template<typename Mutex, typename UseMember>
inline UseMember * ScopeLock<Mutex, UseMember>::operator->()
{
	return m_ref;
}

template<typename Mutex, typename UseMember>
inline const UseMember * ScopeLock<Mutex, UseMember>::operator->() const
{
	return (const UseMember *) m_ref;
}

template<typename Mutex, typename UseMember>
inline void ScopeLock<Mutex, UseMember>::Return()
{
	m_ref = nullptr;
	(m_lock.*m_ptr)();
	m_isUnlocked = true;
}

}

