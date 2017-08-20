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

#include <thread>

namespace cg {
/**A filter interface for readers and writers.*/
class Filter
{
public:
	/**Determine if the size will change when applied.
	\return True if the size will change.*/
	virtual bool SizeChanges() const = 0;
	/**Virtual destructor*/
	virtual ~Filter() {};
	/**Covnert data from one place and store to another.  Dest and source may
	be the same, but may not overlap if they are different pointers.
	\param src The place to read the data from.
	\param size The size of the data destination.
	\return An array view with the converted data.*/
	virtual std::vector<char> TransformCopy(const char* src, std::size_t size)
	{
		std::vector<char> av(src,src+size);
		Filter::Transform(av);
		return av;
	}
	/**Transform data in place (no copies).
	\param av The array view to transform.*/
	virtual void Transform(std::vector<char>& av)
	{
		Transform(av.data(), av.size());
	}
	/**Transform data in place (no copies).
	\param data The data place.
	\param size The data size.*/
	virtual void Transform(char* data, std::size_t size) = 0;
	/**Transform and array view in place.
	\param av The array view to transform.
	\return Another array view.*/
	virtual std::vector<char> TransformCopy(std::vector<char>& av)
	{
		return TransformCopy(av.data(), av.size());
	}
	/**Transform with any set of aprameters as log as there is an overload for
	them.

	\return A future with the result.*/
	template<typename...Args>
	inline void AsyncTransformCopy(Args...args)
	{
		return std::async(std::launch::async, [&]() {
			this->TransformCopy(std::forward<Args>(args)...);
		});
	}
	/**Transform with any set of aprameters as log as there is an overload for
	them.

	\return A future with the result.*/
	template<typename...Args>
	inline void AsyncTransform(Args...args)
	{
		return std::async(std::launch::async, [&]() {
			this->TransformInPlace(std::forward<Args>(args)...);
		});
	}
};

}