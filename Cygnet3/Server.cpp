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

#include "Server.hpp"

namespace cg {
namespace net {



Server::~Server()
{
	Wait();
}

void Server::Start(uint16_t port, double updateSpeed)
{
	if (m_run)
		Stop();
	m_updateSpeed.SetFPS(updateSpeed);
	m_port = port;
	m_serverSocket.Bind(true, m_port);
	m_serverSocket.Listen();
	m_run = true;
	ServerLoop();
}

void Server::Stop()
{
	m_run = false;
	Wait();
}

void Server::ChangeUpdateSpeed(double fps)
{
	m_updateSpeed.SetFPS(fps);
}


void Server::AcceptAll()
{
	while (m_serverSocket.ReadReady())
	{
		m_updateSpeed();
		auto it = m_clients.emplace(m_clients.end(), false);
		auto& sock = m_clients.back();
		bool accepted = m_serverSocket.Accept(sock);
		if (!accepted)
			m_clients.pop_back();
		else
		{
			auto it = m_listeners.begin();
			auto end = m_listeners.end();
			for (; it != end; ++it)
				(*it)->Connected(sock);
		}
	}
}

void Server::ServerLoop()
{
	while (m_run)
	{
		m_updateSpeed();
		AcceptAll();
		CheckClients();
	}
}

void Server::CheckClients()
{
	if (m_clients.size() == 0)
		return;
	auto it = m_clients.begin();
	auto end = m_clients.end();
	for (; it != end; ++it)
	{
		m_updateSpeed();
		/*if its close, erase it and skip it.*/
		int open = it->IsOpen();
		if (open != 1)
		{
			auto it2 = m_listeners.begin();
			auto end = m_listeners.end();
			for (; it2 != end; ++it2)
				(*it2)->Disconnected(*it);

			if (m_clients.size() > 1)
				it = m_clients.erase(it);
			else
			{
				m_clients.erase(it);
				break;
			}
			continue;
		}
		if (it->ReadReady())
		{
			cg::net::SocketRW reader(&(*it),nullptr,&m_readFilters);
			reader.SupressCleanup(true, true);
			auto av = reader.Read();
			auto it2 = m_listeners.begin();
			auto end = m_listeners.end();
			for (; it2 != end; ++it2)
			{
				if (!(*it2)->Received(*it, av))
				{
					auto it3 = m_listeners.begin();
					auto end = m_listeners.end();
					for (; it3 != end; ++it3)
						(*it3)->Disconnected(*it);
					it = m_clients.erase(it);
					break;
				}
			}
		}
		/*if the last one was removed, stop looping. The for loop will cause an
		exception otherwise.*/
		if (m_clients.size() == 0)
			break;
	}
}

}
}