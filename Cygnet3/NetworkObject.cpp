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
#include "NetworkObject.hpp"

namespace cg {
namespace net {


std::size_t NetworkObject::ms_socketsActive = 0;

NetworkObject::NetworkObject()
{
	if (ms_socketsActive++ == 0)
	{
		StartSockets();
	}
}

NetworkObject::~NetworkObject()
{
	if (--ms_socketsActive == 0)
	{
		StopSockets();
	}
}

void NetworkObject::StartSockets()
{
	cg::Logger::LogNote(3, "Network: Starting Sockets.");
	bool success = OSStart();
	cg::Logger::LogError(!success,__FUNCSTR__, "Sockets not started.");
}

void NetworkObject::StopSockets()
{
	cg::Logger::LogNote(3, "Network: Stopping Sockets.");
	OSStop();
}


bool NetworkObject::OSStart()
{
#ifdef _WIN32
	WSAData data = {};
	int error = WSAStartup(MAKEWORD(2, 2), &data);
	if (error != 0)
	{
		cg::Logger::LogError(__FUNCSTR__, "The WSA system could not start.",
			" Probably a windows issue with WSStartup.");
		return false;
	}
	else
		return true;
#endif

}

void NetworkObject::OSStop()
{
#ifdef _WIN32
	WSACleanup();
#endif
}

}
}