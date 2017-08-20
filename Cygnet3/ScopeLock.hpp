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

namespace cg {


/**A returnable class to auto-unlock when scope is lost.*/
template<typename Mutex, typename UseMember = void>
class ScopeLock
{
public:
	/**The FPtr type.*/
	typedef void(Mutex::*FPtr)();
	/**Create the lock
	\param lock A reference to the mutex to call on.
	\param parameter The function poitner to call when the scope is lost.
	\param lockFunc The function to lock with.
	\param ref A pointer to the thing to have access to.*/
	ScopeLock(Mutex& lock,
		FPtr parameter,
		FPtr lockFunc,
		UseMember* ref = nullptr);
	/**The copy ctor
	\param other The other thing to copy.*/
	ScopeLock(const ScopeLock<Mutex, UseMember>& other) = delete;
	/**The move ctor
	\param other The other thing to move.*/
	ScopeLock(ScopeLock<Mutex, UseMember>&& other);
	/**Auto unlock this.*/
	~ScopeLock();
	/**Get the member held by the scope lock.
	\return The member that is tracked.*/
	UseMember* operator->();
	/**Get the member held by the scope lock.
	\return The member that is tracked.*/
	const UseMember* operator->() const;
	/**Un lock the lock and invalidate the access point.*/
	void Return();
private:
	/**The member accesser*/
	UseMember* m_ref;
	/**A reference to the lock*/
	Mutex& m_lock;
	/**The function taht will unlock the lock.*/
	FPtr m_ptr;
	/**premature unlocking*/
	bool m_isUnlocked = false;
};

}

#include "ScopeLock_Impl.hpp"



