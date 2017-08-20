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

#include <list>

#include "SocketRW.hpp"
#include "Timer.hpp"
#include "IServerListener.hpp"
#include "FilterGroup.hpp"
#include "Waitable.hpp"

namespace cg {
namespace net {
/**A single threaded server.*/
class Server : public NetworkObject, public cg::Waitable
{
public:
	/**The virt decon*/
	virtual ~Server();
	/**Start the server.
	\param port The port to connect to.
	\param updateSpeed The amount of times the loop will be run per sec.*/
	virtual void Start(uint16_t port, double updateSpeed = 30);
	/**Stop the server.*/
	virtual void Stop();
	/**Add a listener to the server.
	\param args The args to pass to a new `Listener`.*/
	template<typename Listener, typename...Args>
	void AddListener(Args&&...args);
	/**Add a filter to the data reading process.
	\param args The args to pass to the decryption filter constructor.*/
	template<typename Filter, typename...Args>
	void AddFilter(Args&&...args);
	/**Determine if stopping should happen.
	\return False to continue waiting.*/
	virtual bool ShouldStop() const {
		return !m_run;
	}
protected:
	/**The type of list for holding clients.*/
	using ClientList = std::list<cg::net::Socket>;
	/**Change the update speed of the loop.
	\param fps The speed in frames per sec.*/
	void ChangeUpdateSpeed(double fps);
	/**A list of listeners to process sockets.*/
	std::list<cg::net::IServerListener*> m_listeners;
	/**Accept any sockets that are available to accept.*/
	virtual void AcceptAll();
	/**Basic server loop.*/
	virtual void ServerLoop();
	/**Iterate all sockets and call HandleData on each one that is ready.*/
	virtual void CheckClients();
	/**The server socket.*/
	cg::net::Socket m_serverSocket;
	/**The port to connect to.*/
	uint16_t m_port;
	/**True to run the server components*/
	bool m_run = false;
	/**The client list.*/
	ClientList m_clients;
	/**The update speed of the server.*/
	cg::SpeedLimit m_updateSpeed;
	/**A group of filters that can be applied all at once.*/
	cg::FilterGroup m_readFilters;
};

template<typename Listener, typename ...Args>
inline void Server::AddListener(Args && ...args)
{
	static_assert(std::is_base_of<IServerListener, Listener>::value,
		"`Listener` must be derived from IServerListener.");
	m_listeners.emplace_back(cg::New<Listener>(__FUNCSTR__,
		std::forward<Args>(args)...));
}

template<typename FilterT, typename...Args>
inline void Server::AddFilter(Args && ...args)
{
	static_assert(std::is_base_of<cg::Filter, FilterT>::value,
		"`FilterT` must be derived from cg::Filter.");
	m_readFilters.AddFilter(cg::New<FilterT>(__FUNCSTR__,
		std::forward<Args>(args)...));
}

}
}


