
#include "ServerMT.hpp"

namespace cg {
namespace net {



ServerMT::ServerMT(int dataThreads)
	:m_dataThreadCount(dataThreads)
{
	m_run = false;
}

ServerMT::~ServerMT()
{
	if (!m_serverStopped)
	{
		Stop();
	}
	for (int i = 0; i < m_dataThreadCount; ++i)
	{
		if (m_dataThreads[i])
			cg::Delete(__FUNCSTR__, m_dataThreads[i]);
	}
	cg::DeleteA(__FUNCSTR__, m_dataThreads);
	cg::Delete(__FUNCSTR__, mt_acceptScanner);
	cg::Delete(__FUNCSTR__, mt_readyScanner);
}

void ServerMT::Start(uint16_t port)
{
	if (m_run)
		Stop();
	m_acceptorLimit.SetFPS(100);
	m_readyLimit.SetFPS(100);
	m_run = true;
	m_serverStopped = false;
	m_dataThreads = cg::NewA<std::thread*>(__FUNCSTR__, m_dataThreadCount);
	for (int i = 0; i < m_dataThreadCount; ++i)
	{
		m_dataThreads[i] = cg::New<std::thread>(__FUNCSTR__,
			&ServerMT::DataLoop, this);
	}
	m_serverSocket.Bind(true, port);
	AutoAccept();
	ScanForReady();
}

void ServerMT::Stop()
{
	m_run = false;
	m_readyBox.NotifyAll();
	m_clientBox.NotifyAll();
	CloseAll();
	WaitForThreads();
	m_serverStopped = true;
}


void ServerMT::NotifyAllThreads()
{
	m_readyBox.Notify();
	m_clientBox.Notify();
}

void ServerMT::ChangeAcceptorSpeed(double fps)
{
	m_acceptorLimit.SetFPS(fps);
}

void ServerMT::ChangeScannerSpeed(double fps)
{
	m_readyLimit.SetFPS(fps);
}

void ServerMT::DataLoop()
{
	++m_activeDataThreads;
	while (m_run)
	{
		/*will wait for elements.*/
		if (m_readyBox.Size() == 0)
			m_readyBox.WaitForElements(std::ref(m_run));
		cg::net::Socket* sock = nullptr;
		auto clientWriter = m_clientBox.Writer();
		auto writer = m_readyBox.Writer();
		if (writer->size() == 0)
			continue;
		sock = *writer->begin();
		writer->pop_front();
		if (sock->IsOpen() != 1)
		{
			cg::Delete(__FUNCSTR__, sock);
		}
		/*sock should be ready beause it was in the ready list.*/
		cg::net::SocketRW reader(sock, nullptr, &m_readFilters);
		reader.SupressCleanup(true, true);
		auto av = reader.Read();
		auto it2 = m_listeners.begin();
		auto end = m_listeners.end();
		for (; it2 != end; ++it2)
		{
			if (!(*it2)->Received(*sock, av))
			{
				auto it3 = m_listeners.begin();
				auto end = m_listeners.end();
				for (; it3 != end; ++it3)
					(*it3)->Disconnected(*sock);
				cg::Delete(__FUNCSTR__, sock);
				break;
			}
		}
		/*put it back into the client box.*/
		clientWriter->push_back(sock);
	}
	--m_activeDataThreads;
}

void ServerMT::CloseAll()
{
	bool a = false;
	bool b = false;
	bool c = false;

	cg::SpeedLimit limit(10);
	while (!a || !b || !c)
	{
		limit();
		a = m_readyScannerStopped;
		b = m_acceptScannerStopped;
		c = m_activeDataThreads == 0;
	}
	{
		m_serverSocket.Close();
	}
	{
		auto writer = m_clientBox.Writer();
		auto it = writer->begin();
		auto end = writer->end();
		for (; it != end; ++it)
		{
			(*it)->Close();
			cg::Delete(__FUNCSTR__, *it);
		}
	}
	{
		auto writer = m_readyBox.Writer();
		auto it = writer->begin();
		auto end = writer->end();
		for (; it != end; ++it)
		{
			(*it)->Close();
			cg::Delete(__FUNCSTR__, *it);
		}
	}
	{
		auto it = m_listeners.begin();
		auto end = m_listeners.end();
		for (; it != end; ++it)
		{
			cg::Delete(__FUNCSTR__, *it);
		}
	}
}

void ServerMT::WaitForThreads()
{
	if (mt_readyScanner)
		if (mt_readyScanner->joinable())
			mt_readyScanner->join();
	if (mt_acceptScanner)
		if (mt_acceptScanner->joinable())
			mt_acceptScanner->join();
	for (int i = 0; i < m_dataThreadCount; ++i)
	{
		if (m_dataThreads[i])
			if (m_dataThreads[i]->joinable())
				m_dataThreads[i]->join();
	}
}

void ServerMT::ScanForReady()
{
	m_readyScannerStopped = false;
	mt_readyScanner = cg::New<std::thread>(__FUNCSTR__,
		&ServerMT::ScanForReadyLoop, this);
}

void ServerMT::AutoAccept()
{
	m_acceptScannerStopped = false;
	m_serverSocket.Listen();
	mt_acceptScanner = cg::New<std::thread>(__FUNCSTR__,
		&ServerMT::AcceptLoop, this);
}

void ServerMT::ScanForReadyLoop()
{
	while (m_run)
	{
		/*will wait for elements.*/
		m_readyLimit();
		auto writer = m_clientBox.Writer();
		if (writer->empty())
			continue;
		auto it = writer->begin();
		auto end = writer->end();
		for (; it != end; ++it)
		{
			int open = (*it)->IsOpen();
			if (open != 1)
			{
				auto it2 = m_listeners.begin();
				auto end2 = m_listeners.end();
				for (; it2 != end2; ++it2)
					(*it2)->Disconnected(**it);
				(**it).Close();
				cg::Delete(__FUNCSTR__, *it);
				it = writer->erase(it);
				if (it == end)
					/*If it was the last thing in the list, it now equals .end
					and we will break instead of trying to increment it.*/
					break;
				else
					continue;
			}
			if ((*it)->ReadReady())
			{
				m_readyBox.Writer()->push_back(*it);
				it = writer->erase(it);
				m_readyBox.Notify();
				if (it == end)
					/*If it was the last thing in the list, it now equals .end
					and we will break instead of trying to increment it.*/
					break;
			}
		}
	}
	m_readyScannerStopped = true;
}

void ServerMT::AcceptLoop()
{
	while (m_run)
	{
		m_acceptorLimit();
		while (m_serverSocket.ReadReady())
		{
			auto writer = m_clientBox.Writer();
			auto sock = cg::New<cg::net::Socket>(__FUNCSTR__);
			auto accepted = m_serverSocket.Accept(*sock, false);
			if (accepted)
			{
				auto it2 = m_listeners.begin();
				auto end = m_listeners.end();
				for (; it2 != end; ++it2)
					(*it2)->Connected(*sock);
				writer->push_back(sock);
			}
			else
			{
				/*not accepted for some reason, delete it.*/
				cg::Delete(__FUNCSTR__, sock);
			}
		}
	}
	m_acceptScannerStopped = true;
}

}
}