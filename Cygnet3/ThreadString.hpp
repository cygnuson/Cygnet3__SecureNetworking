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