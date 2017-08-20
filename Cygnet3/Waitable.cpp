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