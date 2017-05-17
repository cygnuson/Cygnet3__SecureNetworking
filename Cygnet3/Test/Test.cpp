
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