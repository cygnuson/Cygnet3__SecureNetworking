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
#include "SocketRW.hpp"

namespace cg {
namespace net {

SocketRW::SocketRW(const Socket* sock,
	Filter * writeFilter,
	Filter * readFilter)
	:m_socket(sock), m_readFilter(readFilter), m_writeFilter(writeFilter)
{
	EnableLogs(true, "SocketRW");
}
SocketRW::SocketRW(SocketRW && other)
	: m_socket(other.m_socket),
	m_writeFilter(other.m_writeFilter),
	m_readFilter(other.m_readFilter)
{
	other.m_readFilter = nullptr;
	other.m_writeFilter = nullptr;
}
void SocketRW::operator=(SocketRW && other)
{
	m_socket = other.m_socket;
	m_readFilter = other.m_readFilter;
	m_writeFilter = other.m_writeFilter;
	other.m_readFilter = nullptr;
	other.m_writeFilter = nullptr;
	other.m_socket = nullptr;
}
SocketRW::SocketRW(const Socket * sock)
	: m_socket(sock),
	m_readFilter(nullptr),
	m_writeFilter(nullptr)
{
	EnableLogs(true, "SocketRW");
}
std::ptrdiff_t SocketRW::Write(const char * data,
	int64_t size,
	cg::Timeout timeoutp)
{
	ptrdiff_t timeout = (ptrdiff_t)timeoutp;
	CheckAndReport();
	if (!WriteReady(timeoutp))
		return 0;

	m_socket->Lock();
	if (!m_writeFilter)
	{
		auto sSize = uint64_t(size);
		/*prepend the size of the data.*/
		m_socket->Send((char*)&sSize, sizeof(uint64_t), true);
		if (!m_writeFilter)
		{
			m_socket->Unlock();
			return m_socket->Send(data, (std::size_t) size, true);
		}
		else
		{
			auto tData =
				m_writeFilter->TransformCopy(data, (std::size_t) size);
			m_socket->Unlock();
			return m_socket->Send(tData.data(), tData.size(), true);
		}
	}
	else
	{
		auto tData =
			m_writeFilter->TransformCopy(data, (std::size_t) size);
		int64_t sSize = tData.size();
		/*prepend the size of the data.*/
		m_socket->Send((char*)&sSize, sizeof(uint64_t), true);
		auto r = m_socket->Send(tData.data(), tData.size(), true);
		m_socket->Unlock();
		return r;
	}
}
std::vector<char> SocketRW::Read(int64_t expectedSize,
	cg::Timeout timeoutp)
{
	auto timeout = (ptrdiff_t)timeoutp;
	CheckAndReport();
	if (!ReadReady(timeoutp))
		return std::vector<char>();

	std::int64_t size = 0;
	m_socket->Lock();
	auto got = m_socket->Recv((char*)&size, sizeof(size), true);
	if (got == -1)
	{
		/*socket closed normally.*/
		LogNote(3, __FUNCSTR__, "Socket closed normally.");
		m_socket->Unlock();
		throw NetworkException(Error::NotConnected);
	}
	std::vector<char> av((std::size_t) size, 0);
	m_socket->Recv(av.data(), (std::size_t) size, true);
	if (m_readFilter)
	{
		if (m_readFilter->SizeChanges())
			av = m_readFilter->TransformCopy(av.data(), (std::size_t)size);
		else
			m_readFilter->Transform(av.data(), (std::size_t) size);

	}
	m_socket->Unlock();
	return av;
}

void SocketRW::SetReaderFilter(cg::Filter * newFilter)
{
	if (m_readFilter)
		cg::Delete(__FUNCSTR__,m_readFilter);
	m_readFilter = newFilter;
}

void SocketRW::SetWriterFilter(cg::Filter * newFilter)
{
	if (m_writeFilter)
		cg::Delete(__FUNCSTR__,m_writeFilter);
	m_writeFilter = newFilter;
}


bool SocketRW::ReadReady(cg::Timeout timeout) const
{
	CheckAndReport();
	return m_socket->ReadReady(timeout);
}

bool SocketRW::WriteReady(cg::Timeout timeout) const
{
	CheckAndReport();
	return m_socket->WriteReady(timeout);
}

void SocketRW::SupressCleanup(bool read, bool write)
{
	m_supressReaderCleanup = read;
	m_supressWriterCleanup = write;
}

void SocketRW::CheckAndReport() const
{
	if (!((const Socket*)m_socket))
	{
		LogError("There is no set socket.");
		throw NetworkException(Error::BadSocket);
	}
}


}
}
