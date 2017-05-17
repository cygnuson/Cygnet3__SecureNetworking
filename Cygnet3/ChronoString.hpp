#pragma once

#include "StringHelpers.hpp"
#include "Tag.hpp"

#include <chrono>
#include <ctime>

namespace cg {

#ifdef __unix__
inline struct tm *
localtime_s(struct tm * result,const time_t * time)
{
    auto t = localtime(time);
    *result = *t;
    return result;
}
#endif


/**Convert a time unit to a string.
\param time The time to convert.
\return The string.*/
inline std::string ToString(std::chrono::duration<double> time)
{
	std::string str = ToString(time.count());
	str += " s";
	return str;
}
/**Convert a time unit to a string.
\param time The time to convert.
\return The string.*/
inline std::string ToString(std::chrono::milliseconds time)
{
	std::string str = ToString(time.count());
	str += " ms";
	return str;
}
/**Convert a time unit to a string.
\param time The time to convert.
\return The string.*/
inline std::string ToString(std::chrono::microseconds time)
{
	std::string str = ToString(time.count());
	str += " us";
	return str;
}
/**Convert a time unit to a string.
\param time The time to convert.
\return The string.*/
inline std::string ToString(std::chrono::nanoseconds time)
{
	std::string str = ToString(time.count());
	str += " ns";
	return str;
}
/**Convert a time unit to a string.
\param time The time to convert.
\return The string.*/
inline std::string ToString(std::chrono::minutes time)
{
	std::string str = ToString(time.count());
	str += " min";
	return str;
}
/**Convert a time unit to a string.
\param time The time to convert.
\return The string.*/
inline std::string ToString(std::chrono::hours time)
{
	std::string str = ToString(time.count());
	str += " hr";
	return str;
}
/**Get a string that is the current date.
\return The current date as a string.*/
inline std::string ToString(cg::Tags::CurrentTimeTag)
{
	std::stringstream time;
	time << "[";
	std::time_t t = std::time(0);
	std::tm now;
    localtime_s(&now, &t);
	time << std::setw(2) << std::setfill('0') << now.tm_hour;
	time << ":" << std::setw(2) << std::setfill('0') << now.tm_min;
	time << ":" << std::setw(2) << std::setfill('0') << now.tm_sec;
	time << "] " << std::flush;
	return time.str();
}

}
