#pragma once

#include <future>
#include <vector>

#include "LogAdaptor.hpp"

#define _DEBUGREADER _DEBUG && 1

namespace cg {

/**An interface class to facilitate read of various locations.*/
class Reader : cg::LogAdaptor<Reader>
{
public:
	/**Determine if the object is in a state that would allow it to send.
	\param timeout The time to wait untill returning. Use cg::Timeout or
	cg::MakeTimeout
	\return True if the object is ready to be written to without blocking.*/
	virtual bool ReadReady(cg::Timeout timeout = cg::Timeout::None) const = 0;
	/**Read from the object.
	\param timeout The time to wait untill returning. Use cg::Timeout or
	cg::MakeTimeout
	\param expectedSize The max size to get. Might be less
	\return an ArrayView with data.  An ArrayView with nullptr and 0 size if
	no bytes are read.*/
	virtual std::vector<char> Read(int64_t expectedSize,
		cg::Timeout timeout = cg::Timeout::None) = 0;
	/**Read some data. T must have a member .data()  that will return a
	pointer to a location to store data. T must also have a member .size()const
	that will return the size of the data to be stored.
	\tparam T An object that has .data()  and .size() const for storing
	data.
	\param data An object of type T that has .data()  and .size() const .
	\param timeout The time to wait untill returning. Use cg::Timeout or
	cg::MakeTimeout
	\return The amount of bytes read.*/
	template<typename T>
	inline std::ptrdiff_t Read(T & data, cg::Timeout timeout = 
		cg::Timeout::None)
	{
		return Read(data.data(), data.size(), timeout);
	}
	/**Read data in async mode. 

	\tparam T An object that has .data()  and .size() const for reading
	its data.
	\return A future containing the amount of bytes written.*/
	template<typename...Args>
	inline std::future<std::ptrdiff_t> AsyncRead(Args&&...args)
	{
		return std::async(std::launch::async, [&]() {
			return this->Read(std::forward<Args>(args)...);
		});
	}
protected:
	using cg::LogAdaptor<Reader>::EnableLogs;
	using cg::LogAdaptor<Reader>::LogNote;
	using cg::LogAdaptor<Reader>::LogWarn;
	using cg::LogAdaptor<Reader>::LogError;
	using cg::LogAdaptor<Reader>::Log;
	using cg::LogAdaptor<Reader>::ms_log;
	using cg::LogAdaptor<Reader>::ms_name;

};

}