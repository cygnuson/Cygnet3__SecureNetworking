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

#include "SecureHelpers.hpp"
#include "Filter.hpp"
#include "Logger.hpp"

namespace cg {
/**A filter for auto decrypting data.*/
class RSADecryptFilter : public cg::Filter
{
public:
	virtual ~RSADecryptFilter() {};
	/**always returns tre because the size will always change.*/
	virtual bool SizeChanges() const
	{
		return true;
	}
	/**Create the filter.

	\param key The private key.*/
	RSADecryptFilter(CryptoPP::RSA::PrivateKey key)
		:m_key(key) {};
	/**Create the filter.

	\param keys The pair of keys.*/
	RSADecryptFilter(cg::KeyPair keys)
		:m_key(keys.m_private) {};
	/**Transform data in place (no copies).
	\param data The data place.
	\param size The data size.*/
	virtual void Transform(char* data, std::size_t size)
	{
		cg::Logger::LogError("Cannot transform RSA in place. The destination",
			" must be different.");
		throw ExEncryption(ExEncryption::Code::BadMem);
	}
	/**Covnert data from one place and store to another.  Dest and source may
	be the same, but may not overlap if they are different pointers.
	\param src The place to read the data from.
	\param size The size of the data destination.
	\return An array view with the converted data.*/
	virtual ArrayView TransformCopy(const char* src, std::size_t size) override
	{
		auto ret = cg::SecureHelpers::RSADecrypt(src, size, m_key);
		ArrayView av(ret.size());
		std::memcpy(av.data(), ret.data(), ret.size());
		return av;
	}
private:
	/**A pointer to the key. might be public, might be a private key.*/
	CryptoPP::RSA::PrivateKey m_key;

};
/**A filter for auto encrypting data.*/
class RSAEncryptFilter : public cg::Filter
{
public:
	virtual ~RSAEncryptFilter() {};
	/**always returns tre because the size will always change.*/
	virtual bool SizeChanges() const
	{
		return true;
	}
	/**Create the filter.

	\param key The public key.*/
	RSAEncryptFilter(CryptoPP::RSA::PublicKey key)
		:m_key(key) {};
	/**Create the filter.

	\param keys The pair of keys.*/
	RSAEncryptFilter(cg::KeyPair keys)
		:m_key(keys.m_public) {};
	/**Covnert data from one place and store to another.  Dest and source may
	be the same, but may not overlap if they are different pointers.
	\param src The place to read the data from.
	\param size The size of the data destination.
	\return An array view with the converted data.*/
	virtual ArrayView TransformCopy(const char* src, std::size_t size) override
	{
		auto ret = cg::SecureHelpers::RSAEncrypt(src, size, m_key);
		ArrayView av(ret.size());
		std::memcpy(av.data(), ret.data(), ret.size());
		return av;
	}
	/**Transform data in place (no copies).
	\param data The data place.
	\param size The data size.*/
	virtual void Transform(char* data, std::size_t size)
	{
		cg::Logger::LogError("Cannot transform RSA in place. The destination",
			" must be different.");
		throw ExEncryption(ExEncryption::Code::BadMem);
	}
private:
	/**A pointer to the key. might be public, might be a private key.*/
	CryptoPP::RSA::PublicKey m_key;
};

}