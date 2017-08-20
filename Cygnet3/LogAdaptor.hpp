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

#include "Logger.hpp"

namespace cg {
/**An adaptor for class scope log functions.*/
template<typename T>
class LogAdaptor
{
public:
	/*Direct pass to the logger library except that its filtered based on the
	flag that is set.
	\param args Arguments that pass directly to the underlying log library.
	\param level The log level to use for this message.
	\sa cg::Logger::Log*/
	template<typename...Ts>
	static void Log(cg::Logger::Level level,
		Ts...args);
	/**Direct pass to the logger library except that its filtered based on the
	flag that is set.
	\param args Arguments that pass directly to the underlying log library.
	\sa cg::Logger::LogError*/
	template<typename...Ts>
	static void LogError(Ts...args);
	/**Enable the logging options for the subclass of the object.
	\param set True to enable the logging, or false to disable it.
	\param name The name to set for logging.*/
	static void EnableLogs(bool set,
		std::string&& name);
	/**Enable the logging options for the subclass of the object.
	\param set True to enable the logging, or false to disable it.*/
	static void EnableLogs(bool set);
	/**Direct pass to the logger library except that its filtered based on the
	flag that is set.  Expects the first argument to be 1,2, or 3 to 
	determine what level the message will be displayed for.
	\param noteLevel The e_noteLevel that the note will appear on.
	\param args Arguments that pass directly to the underlying log library.
	\sa cg::Logger::LogNote*/
	template<typename...Ts>
	static void LogNote(int noteLevel,
		Ts...args);
	/**Direct pass to the logger library except that its filtered based on the
	flag that is set.
	\param args Arguments that pass directly to the underlying log library.
	\sa cg::Logger::LogWarn*/
	template<typename...Ts>
	static void LogWarn(Ts...args);
protected:/********************************************************************PROTECTED**********/
	static bool ms_log; ///< logging flag
	static std::string ms_name; ///<The name of the class using the logs.
};

template<typename T>
bool LogAdaptor<T>::ms_log = false;
template<typename T>
std::string LogAdaptor<T>::ms_name = "NONAME";


/*Direct pass to the logger library except that its filtered based on the
flag that is set.
\param args Arguments that pass directly to the underlying log library.
\param level The log level to use for this message.
\sa cg::Logger::Log*/
template<typename T>
template<typename ...Ts>
inline void LogAdaptor<T>::Log(cg::Logger::Level level,
	Ts ...args)
{
	cg::Logger::Log(ms_log, level, ms_name, ":", 
		std::forward<Ts>(args)...);
}


/**Direct pass to the logger library except that its filtered based on the
flag that is set.
\param args Arguments that pass directly to the underlying log library.
\sa cg::Logger::LogError*/
template<typename T>
template<typename ...Ts>
inline void LogAdaptor<T>::LogError(Ts ...args)
{
	cg::Logger::LogError(ms_log, ms_name, ":", 
		std::forward<Ts>(args)...);
}


/**Direct pass to the logger library except that its filtered based on the
flag that is set.  Expects the first argument to be 1,2, or 3 to
determine what level the message will be displayed for.
\param noteLevel The e_noteLevel that the note will appear on.
\param args Arguments that pass directly to the underlying log library.
\sa cg::Logger::LogNote*/
template<typename T>
template<typename ...Ts>
inline void LogAdaptor<T>::LogNote(int noteLevel,
	Ts ...args)
{
	cg::Logger::LogNote(ms_log, noteLevel,
		ms_name, ":", std::forward<Ts>(args)...);
}


/**Direct pass to the logger library except that its filtered based on the
flag that is set.
\param args Arguments that pass directly to the underlying log library.
\sa cg::Logger::LogWarn*/
template<typename T>
template<typename ...Ts>
inline void LogAdaptor<T>::LogWarn(Ts ...args)
{
	cg::Logger::LogWarn(ms_log, ms_name, ":", std::forward<Ts>(args)...);
}

/**Enable the logging options for the subclass of the object.
\param set True to enable the logging, or false to disable it.*/
template<typename T>
void LogAdaptor<T>::EnableLogs(bool set)
{
	/*enable or disable the log.*/
	ms_log = set;
}
/**Enable the logging options for the subclass of the object.
\param set True to enable the logging, or false to disable it.
\param name The name to set for logging.*/
template<typename T>
inline void LogAdaptor<T>::EnableLogs(bool set, 
	std::string && name)
{
	/*enable or disable the log.*/
	ms_log = set;
	/*set the name*/
	ms_name = name;
}


}