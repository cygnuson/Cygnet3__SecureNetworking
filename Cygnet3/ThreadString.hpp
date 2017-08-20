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

#include "StringHelpers.hpp"

#include <thread>

namespace cg {

/**Create a string from a thread id.
\param id The threadid to convert.
\return The string.*/
inline std::string ToString(const std::thread::id& id)
{
	std::stringstream ss;
	ss << id;
	return ss.str();
}
/**Create a string from the current thread id.
\return The string.*/
inline std::string ToString(cg::Tags::ThreadTag)
{
	std::stringstream ss;
	ss << std::this_thread::get_id();
	return ss.str();
}

}