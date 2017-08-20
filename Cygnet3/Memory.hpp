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

#include <cstdlib>
#include <string>

#include "Logger.hpp"
#include "exception.hpp"

#if defined(_DEBUG)
#include <map>
#endif

namespace cg {

#if defined(_DEBUG)


template<typename T>
class DataLeakImpl;

template<typename T>
class DataLeakImpl
{
public:
	/**The amount of bytes allocated.*/
	static std::ptrdiff_t m_allocated;
	/**The total amount of allocated memory, including what has been freed.*/
	static std::ptrdiff_t m_totalAllocated;
	/**Get the peek ram usuage.*/
	static std::ptrdiff_t m_peekRam;
	/**A list of allocator records.*/
	static std::map<void*, std::pair<std::string, std::size_t>> m_records;

};

template<typename T>
std::ptrdiff_t DataLeakImpl<T>::m_allocated = 0;

template<typename T>
std::ptrdiff_t DataLeakImpl<T>::m_totalAllocated = 0;

template<typename T>
std::ptrdiff_t DataLeakImpl<T>::m_peekRam = 0;

template<typename T>
std::map<void*, std::pair<std::string, std::size_t>> DataLeakImpl<T>::m_records;

using DataLeak = DataLeakImpl<int>;

#endif
/**Allocate a unit of T.
\param args The args that will pass to the constructor.
\return A poitner to the new object.*/
template<typename T, typename...Args>
static T* New(const std::string& note, Args&&...args);
/**Allocate a unit of T.
\param args The args that will pass to the constructor.
\param loc The place to do placement construction.
\return A poitner to the new object.*/
template<typename T, typename...Args>
static T* PNew(const std::string& note, T* loc, Args&&...args);
/**Allocate some space without constructing anything.
\param units The amount of units of T to allocate space for.
\param init True to init the memory.
\return A pointer to the allocated space (as void*).*/
template<typename T>
static T* NewA(const std::string& note, std::size_t units, bool init = false);

/**Deallocate some memory. If it is an array, that is detected properly.
Will only work for pointers allocated with this allocator.
\param loc A pointer to the memory to deallocate.*/
template<typename T>
static void Delete(const std::string& note, T* loc);
/**Deallocate some memory. If it is an array, that is detected properly.
Will only work for pointers allocated with this allocator.
\param loc A pointer to the memory to deallocate.*/
template<typename T>
static void DeleteA(const std::string& note, T* loc);
/**Balance the budget.  Will list all the memory that is currently
allocated. Any leaks at this point will show.
\return The amount of allocated bytes.*/

/**Get the number of currently allocated bytes.*/
static std::size_t MemoryBalance();
/**Determine the enire amount of memory that has been allocated, including
that which has been freed.
\return The total ram usage.*/
static std::size_t TotalMemoryUsage();
/**Determine the the highest amount of ram allocated at one time.
\return The total ram usage.*/
static std::size_t PeekMemoryUsage();
/**Get a memory report.
\return A string with the memory report.*/
static std::string MemoryReport();
}


#include "Memory_impl.hpp"


