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
namespace ex{
	/**A generic exception class for inhariting.*/
class Exception : public cg::Stringable, public std::exception
{
public:
	/**Translate the error to a message that can be read.
	
	Tradition what method for an exception.
	\@return A string that describes the issue at hand.*/
	virtual std::string What() const
	{
		return ToString();
	}
	/**Translate the error to a message that can be read.
	
	\return A string that shows the value of the object.*/
	virtual std::string ToString() const = 0;
	/**\sa std::exception::what
	\return Same as What()*/
	virtual const char* what() const
	{
		return What().c_str();
	}
protected:/********************************************************************PROTECTED**********/

};
/**Incorrectly deleting an array.*/
class ExDeleteArray : public cg::ex::Exception
{
	/**Translate the error to a message that can be read.

	\return A string that shows the value of the object.*/
	inline virtual std::string ToString() const override
	{
		return "Attempted to delete an array with non-array methods.";
	}

};
/**An exception for when a non null pointer was expected, but NULL was
received.*/
class ExNullPointer : public cg::Stringable, public std::exception
{
public:
	/**Destructor*/
	virtual ~ExNullPointer() noexcept {}
	/**Translate the error to a message that can be read.

	\return A string that shows the value of the object.*/
	virtual std::string ToString() const;
};
/**An exception for functions that have not been implemented.*/
class ExNotImplemented : public cg::Stringable, public std::exception
{
public:
	/**Destructor*/
	virtual ~ExNotImplemented() noexcept {}
	/**Translate the error to a message that can be read.

	\return A string that shows the value of the object.*/
	virtual std::string ToString() const;
private:

};
/**An exception for when some sort of index is out of bounds.*/
class ExIndexBounds : public cg::Stringable, public std::exception
{
public:
	/**Destructor*/
	virtual ~ExIndexBounds() noexcept {}
	/**Translate the error to a message that can be read.

	\return A string that shows the value of the object.*/
	virtual std::string ToString() const;
private:

};
/**An exception for when something requires ownership, but does not have it.*/
class ExDoesNotOwn : public cg::Stringable, public std::exception
{
public:
	/**Destructor*/
	virtual ~ExDoesNotOwn() noexcept {}
	/**Translate the error to a message that can be read.

	\return A string that shows the value of the object.*/
	virtual std::string ToString() const;
private:

};
/**An exception for when something is in a running state and not supposed to be
*/
class ExStillRunning : public cg::Stringable, public std::exception
{
public:
	/**Destructor*/
	virtual ~ExStillRunning() noexcept {}
	/**Translate the error to a message that can be read.

	\return A string that shows the value of the object.*/
	virtual std::string ToString() const;
private:

};
/**An exception for when something is not in a running state and supposed to be
*/
class ExNotRunning : public cg::Stringable, public std::exception
{
public:
	/**Destructor*/
	virtual ~ExNotRunning() noexcept {}
	/**Translate the error to a message that can be read.

	\return A string that shows the value of the object.*/
	virtual std::string ToString() const;
private:

};


}
}