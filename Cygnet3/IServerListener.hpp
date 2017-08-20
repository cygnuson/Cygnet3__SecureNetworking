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

#include "Socket.hpp"
#include "Memory.hpp"

namespace cg {
namespace net {
/**An interface for the a lister object for receiving data from a server. All
implementing server listeners shall expect that all data sent to it should have
its data preceided by the size of the data in 8 bytes.  A cg::SocketRW will
automatically prepend the size.*/
class IServerListener : cg::net::NetworkObject
{
public:
	/**The destructor that will clean up the encryptor if needed.*/
	virtual ~IServerListener() {}
	/**Function for when an object is received.
	\param sock A reference to the socket that is ready to receive.
	\param data The data received.
	\return True if the socket is still open.*/
	virtual bool Received(Socket& sock,const std::vector<char>& data) = 0;
	/**Fuction for when a socket is connected.
	\param sock A reference to the socket that just connected.*/
	virtual void Connected(Socket& sock) = 0;
	/**Fuction for when a socket is disconnected.
	\param sock A reference to the socket that just disconnected.*/
	virtual void Disconnected(Socket& sock) = 0;
private:

};

}
}