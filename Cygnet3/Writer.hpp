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

#include <future>
#include <vector>

#include "LogAdaptor.hpp"

namespace cg {

/**An interface class to facilitate writing to various locations.*/
class Writer : cg::LogAdaptor<Writer>
{
public:
	/**Determine if the object is in a state that would allow it to send.
	\param timeout The time to wait untill returning. Use cg::Timeout or
	cg::MakeTimeout
	\return True if the object is ready to be written to without blocking.*/
	virtual bool WriteReady(cg::Timeout timeout =
		cg::Timeout::None) const = 0;
	/**Write some data to the object.  If Ready() returned true before this
	call, the timeout should never be reached sense this object should return
	immediatly.
	\param data A pointer to the data to write.
	\param size The size of the data to write.
	\param timeout The time to wait untill returning. Use cg::Timeout or
	cg::MakeTimeout
	\return The amount of bytes written.*/
	virtual std::ptrdiff_t Write(const char* data,
		int64_t size,cg::Timeout timeout = cg::Timeout::None) = 0;
	/**Write an array view.
	\param av The array view.
	\param timeout The time to wait untill returning. Use cg::Timeout or
	cg::MakeTimeout*/
	virtual void Write(const std::vector<char>& av,
		cg::Timeout timeout = cg::Timeout::None)
	{
		Write(av.data(), av.size(), timeout);
	}
	/**Write some data. T must have a member .data() const that will return a
	pointer to a location to read from. T must also have a member .size() const
	that will return the size of the data to be read.
	\tparam T An object that has .data() const and .size() const for reading
	its data.
	\param data An object of type T that has .data() const and .size() const .
	\param timeout The time to wait untill returning. Use cg::Timeout or
	cg::MakeTimeout
	\return The amount of bytes written.*/
	template<typename T>
	inline std::ptrdiff_t Write(const T & data, cg::Timeout timeout =
		cg::Timeout::None)
	{
		return Write(data.data(), data.size(), timeout);
	}
	/**Write data in async mode.

	\return A future containing the amount of bytes written.*/
	template<typename...Args>
	inline std::future<std::ptrdiff_t> AsyncWrite(Args&&...args)
	{
		return std::async(std::launch::async, [&]() {
			return this->Write(std::forward<Args>(args)...);
		});
	}
protected:
	using cg::LogAdaptor<Writer>::EnableLogs;
	using cg::LogAdaptor<Writer>::LogNote;
	using cg::LogAdaptor<Writer>::LogWarn;
	using cg::LogAdaptor<Writer>::LogError;
	using cg::LogAdaptor<Writer>::Log;
	using cg::LogAdaptor<Writer>::ms_log;
	using cg::LogAdaptor<Writer>::ms_name;
};


}