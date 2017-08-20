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

#include "Error.hpp"
#include "NoCopyMove.hpp"
#include "Logger.hpp"

namespace cg {
namespace net {

/**The network object is the main base class for anything socket related. When
any network objects or objects derived from a networke object exist, the socket
system will be initialized (windows junk).  It has no effect on linux.*/
class NetworkObject : cg::NoCopy
{
public:
	/**The default ctor. Will start the sockets system if needed.*/
	NetworkObject();
	virtual ~NetworkObject();
protected:
	/**Will initialize the sockets system.*/
	static void StartSockets();
	/**Will shutdown the sockets system.*/
	static void StopSockets();
	/**Static counter for the amount of active sockets.*/
	static std::size_t ms_socketsActive;
	/**Start the socket subsystem, does nothing on linux.*/
	static bool OSStart();
	/**Stop the socket system, does nothing on linux.*/
	static void OSStop();
};

struct NetworkInitializer : NetworkObject {};

}
}
