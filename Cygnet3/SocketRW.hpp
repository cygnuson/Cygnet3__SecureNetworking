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
#include "Filter.hpp"
#include "Writer.hpp"
#include "Reader.hpp"

namespace cg {
namespace net {
/**Do some socket Reading and Writing. The size of data sent is prepended 
to the send data. SocketRW::Read can only ready data sent with its size sent
first (as std::uint64_t).*/
class SocketRW : 
	public cg::Writer,
	public cg::Reader,
	private cg::NoCopy,
	public cg::LogAdaptor<SocketRW>
{
public:
	/**Create the socket reader writer.  This object will own its filters and
	they will be clened up on destruction.
	\param sock An address to the socket to use for writing and reading.
	\param writeFilter The filter to be applied to written data.
	\param readFilter The filter ot be applied to the read data.*/
	SocketRW(const Socket* sock,
		Filter* writeFilter,
		Filter* readFilter);
	/**Destroy and clean up*/
	~SocketRW()
	{
		if (m_readFilter && !m_supressReaderCleanup)
			cg::Delete(__FUNCSTR__,m_readFilter);
		if (m_writeFilter && !m_supressWriterCleanup)
			cg::Delete(__FUNCSTR__,m_writeFilter);
	}
	/**Move*/
	SocketRW(SocketRW&& other);
	/**Move*/
	void operator=(SocketRW&& other);
	/**Create the socket reader writer.
	\param sock Aa address to the socket to use for writing and reading.*/
	SocketRW(const Socket* sock);
	/**Write some data to the object.  If Ready() returned true before this
	call, the timeout should never be reached sense this object should return
	immediatly.
	\param data A pointer to the data to write.
	\param size The size of the data to write.
	\param timeout The time to wait untill returning. Use cg::Timeout or
	cg::MakeTimeout
	\return The amount of bytes written. -1 if the socket is not open, or 0
	if nothing was sent.*/
	virtual std::ptrdiff_t Write(const char * data,
		int64_t size, cg::Timeout timeout = cg::Timeout::None) override;
	/**Read from the socket. Will determine size automatically.
	\param timeout The time to wait untill returning. Use cg::Timeout or
	cg::MakeTimeout
	\param expectedSize Does not matter for this class.
	\return an ArrayView with data.  An ArrayView with nullptr and 0 size if
	no bytes are read, or the socket is not open.*/
	std::vector<char> Read(int64_t expectedSize = 0, cg::Timeout timeout =
		cg::Timeout::None);
	/**Set or update the reader filter ascociated with the socketRW.
	\param newFilter The filter to be set. The filter object will become the 
	property of the reader/writer and be deleted when the object destructs.*/
	void SetReaderFilter(cg::Filter* newFilter);
	/**Set or update the writer filter ascociated with the socketRW.
	\param newFilter The filter to be set. The filter object will become the 
	property of the reader/writer and be deleted when the object destructs.*/
	void SetWriterFilter(cg::Filter* newFilter);
	/**Check and see if the socket has data available.
	\param timeout The time to wait untill returning. Use cg::Timeout or
	cg::MakeTimeout
	\return True if at least one byte can be read without blocking.*/
	virtual bool ReadReady(cg::Timeout timeout = cg::Timeout::None) const;
	/**Check and see if the socket can send data without blocking.
	\param timeout The time to wait untill returning. Use cg::Timeout or
	cg::MakeTimeout
	\return True if at least one byte can be written without blocking.*/
	virtual bool WriteReady(cg::Timeout timeout = cg::Timeout::None) const;
	/**Set the status of the filter cleanup.
	\param read True to supress the read filter cleanup.
	\param write True to supress the write filter cleanup.*/
	void SupressCleanup(bool read, bool write);
private:
	using cg::LogAdaptor<SocketRW>::EnableLogs;
	using cg::LogAdaptor<SocketRW>::LogNote;
	using cg::LogAdaptor<SocketRW>::LogWarn;
	using cg::LogAdaptor<SocketRW>::LogError;
	using cg::LogAdaptor<SocketRW>::Log;
	using cg::LogAdaptor<SocketRW>::ms_log;
	using cg::LogAdaptor<SocketRW>::ms_name;
	/**Check and err*/
	void CheckAndReport() const;
	/**Allow for the supression of the filter clean ups.*/
	bool m_supressReaderCleanup = false;
	/**Allow for the supression of the filter clean ups.*/
	bool m_supressWriterCleanup = false;
	/**A ref to the socket.*/
	const Socket* m_socket;
	/**The reading filter filter.*/
	Filter* m_readFilter;
	/**The writing filter filter.*/
	Filter* m_writeFilter;
};

}
}


