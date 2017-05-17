#pragma once

#include "LogAdaptor.hpp"
#include "SocketRW.hpp"
#include "Timer.hpp"
#include "LockBox.hpp"
#include "Memory.hpp"
#include "IServerListener.hpp"
#include "FilterGroup.hpp"
#include "Waitable.hpp"

namespace cg {
namespace net {
/**A multithreaded server.*/
class ServerMT : public NetworkObject, public cg::Waitable
{
public:
	/**The type of list for the clients.*/
	using ClientList = std::list<cg::net::Socket*>;
	/**Default construct
	\param dataThreads The amount of threads that will receive and process
	data from the clients.*/
	ServerMT(int dataThreads = 5);
	/**Virt decon*/
	virtual ~ServerMT();
	/**Start the server.
	\param port The port to start on.*/
	void Start(uint16_t port);
	/**Stop the server and wait for all the threads to stop.*/
	void Stop();
	/**Notify all waiting things managed by this server.*/
	void NotifyAllThreads();
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
		return m_serverStopped;
	}
protected:
	/**Change the speed of the acceptor loop (default 100 FPS).
	\param fps The FPS of the loop.*/
	void ChangeAcceptorSpeed(double fps);
	/**Change the speed of the ready-check loop (default 100 FPS).
	\param fps The FPS of the loop.*/
	void ChangeScannerSpeed(double fps);
	/**Loop through the active sockets and process data.*/
	void DataLoop();
	/**Close all open threads with grace.*/
	void CloseAll();
	/**Wait for all the threads to finish before returning.*/
	void WaitForThreads();
	/**Scan the client list and look for clients that are ready to be read 
	from.*/  
	void ScanForReady();
	/**Continuiously try to accept a new socket.*/
	void AutoAccept();
private:
	/**The loop that will run to scan for clients.*/
	void ScanForReadyLoop();
	/**The loop for checking for ready sockets.*/
	void AcceptLoop();
	/**A list of listeners to process sockets.*/
	std::list<cg::net::IServerListener*> m_listeners;
	/**The speed limit of the acceptor loop. 100 is usually good unless it 
	needs to be faster (in FPS).*/
	cg::SpeedLimit m_acceptorLimit;
	/**The speed of the ready scanning loop. 100 is usually good unless it must
	be faster (in FPS).*/
	cg::SpeedLimit m_readyLimit;
	/**The locked access box for the clients list.*/
	cg::LockBox<ClientList> m_clientBox;
	/**The locked access box for the ready list.*/
	cg::LockBox<ClientList> m_readyBox;
	/**The initial server socket.*/
	cg::net::Socket m_serverSocket;
	/**The master loop controller.*/
	std::atomic_bool m_run;
	/**The port for which to start on.*/
	uint16_t m_port;
	/**The thread that will scan for clients that are ready.*/
	std::thread* mt_readyScanner = nullptr;
	/**The thread that will auto accept connections.*/
	std::thread* mt_acceptScanner = nullptr;
	/**True if the accept thread is notrunning.*/
	bool m_acceptScannerStopped = true;
	/**True if the ready thread has stopped.*/
	bool m_readyScannerStopped = true;
	/**The amount of threads for processing data.*/
	int m_dataThreadCount;
	/**The threads for data processing.*/
	std::thread** m_dataThreads;
	/**The amount of active data threads*/
	int m_activeDataThreads = 0;
	/**True if the server has been stopped.*/
	bool m_serverStopped = true;
	/**A group of filters that can be applied all at once.*/
	cg::FilterGroup m_readFilters;
};

template<typename Listener, typename ...Args>
inline void ServerMT::AddListener(Args && ...args)
{
	static_assert(std::is_base_of<IServerListener, Listener>::value,
		"`Listener` must be derived from IServerListener.");
	m_listeners.emplace_back(cg::New<Listener>(__FUNCSTR__,
		std::forward<Args>(args)...));
}

template<typename FilterT, typename ...Args>
inline void ServerMT::AddFilter(Args && ...args)
{
	static_assert(std::is_base_of<cg::Filter, FilterT>::value,
		"`FilterT` must be derived from cg::Filter.");
	m_readFilters.AddFilter(cg::New<FilterT>(__FUNCSTR__,
		std::forward<Args>(args)...));
}

}
}