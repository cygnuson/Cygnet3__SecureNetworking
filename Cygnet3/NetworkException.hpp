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

#include <string>
#include <cstring>

#include "Exception.hpp"

#include "Error.hpp"

namespace cg {
namespace net {
/**An exception for making the socket error s portable and thorable.*/
class NetworkException : public cg::ex::Exception
{
public:
	/**Create the exception with whatever code errno has currently.
	
	Create a generic exception that will automatically set the erro code to
	be whever the current errno is set as.  Convient cross platform codes
	can be used in the enum NetworkError.
	\sa cg::net::NetworkError*/
	NetworkException() :m_error(GetErrno()) {};
	/**Create the exception with whatever code errno has currently.
	
	Create a generic exception with a user defined error code. Only use this
	if really needed. Prefer to use the default constructor which will set
	the error on its own straight from the socket subsystem.
	\param errorCode The code to be set as the exception value.
	\sa cg::net::NetworkError*/
	NetworkException(Error errorCode);
	/**Copy an exception.
	
	\param other The other exception to copy.
	\sa cg::net::NetworkError*/
	NetworkException(const NetworkException& other);
	/**************************************************************************VIRTUALS***********/
	/**Get a formatted message describing the issue.
	
	A convient function to make the exception fallow the generic exception
	convenctions.  Calls the ToString function only.
	\sa cg::net::NetworkException::ToString
	\sa cg::net::NetworkError*/
	virtual std::string What() const;
	/***************************************************************************STATIC************/
	/**Get the errno number on windows or linux.
	
	\return The errno as a convient and portable cg::net::NetworkError.
	\sa cg::net::NetworkError*/
	static Error GetErrno();
	/**Translate an error code into english.
	
	Get a formatted message from the operating system that will describe any
	issue that happened with the socket system in detail.
	\param error The error code to translate.
	\return A std::string that contains all the data delivered from the 
	operating system call to strerror_s (linux) or FormatMessage (windows).
	\sa cg::net::NetworkError*/
	static std::string ToString(Error error);
	/**Translate an error code into english.
	
	Get a formatted message from the operating system that will describe any
	issue that happened with the socket system in detail.
	\return A std::string that contains all the data delivered from the 
	operating system call to strerror_s (linux) or FormatMessage (windows).
	\sa cg::net::NetworkError*/
	virtual std::string ToString() const;
	/**Get the code.
	\return The code for the exception.*/
	inline Error GetCode() const
	{
		return m_error;
	}
private:/**********************************************************************PRIVATE************/
	/** The error code.*/
	const Error m_error;
};

}
}