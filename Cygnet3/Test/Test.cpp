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

#include "../ServerMT.hpp"


class SL : public cg::net::IServerListener
{
public:


	// Inherited via IServerListener
	virtual bool Received(cg::net::Socket & sock,
		const std::vector<char>& av) override
	{
		std::cout << "\n\n\nReceivabel\n\n\n";
		std::cout << "\n\n\n" << av.data() << "\n\n\n";
		return true;
	}

	virtual void Connected(cg::net::Socket & socket) override
	{
		std::cout << "\n\n\nConnected\n\n\n";
	}

	virtual void Disconnected(cg::net::Socket & socket) override
	{
		std::cout << "\n\n\nDisconnected\n\n\n";
	}

};

class FL : public cg::Filter
{
public:


	// Inherited via Filter
	virtual bool SizeChanges() const override
	{
		return false;
	}

	virtual void Transform(char * data, std::size_t size) override
	{
		for (int i = 0; data[i] != 0; ++i)
			++data[i];
	}

};

int main()
{
	cg::Logger::Init(cg::Logger::e_All, std::cout);

	{
		cg::net::ServerMT s;
		s.AddListener<SL>();
		s.AddListener<SL>();
		s.AddFilter<FL>();
		//auto lam = [&]() {
		s.Start(33899);
		//};
		//std::thread t(lam);
		{
			cg::net::Socket sock;
			sock.Connect("::1", 33899);
			cg::net::SocketRW wt(&sock);
			wt.Write("Hello", 6);
			sock.Close();
		}
		{
			cg::net::Socket sock;
			sock.Connect("::1", 33899);
			cg::net::SocketRW wt(&sock);
			wt.Write("Hello", 6);
			sock.Close();
		}
		s.WaitFor(std::chrono::seconds(1));
		s.Stop();
		//t.join();
	}
	cg::Logger::LogWarn(cg::MemoryReport());
	int stop = 0;
	return 0;
}