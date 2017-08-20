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

namespace cg {
/**A filter to auto-encrypt data with AES.*/
class AESEncryptFilter : public cg::Filter
{
public:
	/**always returns false because the size will never change.*/
	virtual bool SizeChanges() const
	{
		return false;
	}
	/**Create it.
	\param key The key for encryption
	\param iv The IV for encryption*/
	AESEncryptFilter(const CryptoPP::SecByteBlock& key,
		const CryptoPP::SecByteBlock& iv)
		:m_key(key), m_iv(iv) {}
	/**Transform data in place (no copies).
	\param data The data place.
	\param size The data size.*/
	virtual void Transform(char* data, std::size_t size)
	{
		cg::SecureHelpers::AESEncrypt(data, data, size, size, m_key, m_iv);
	}
private:
	/**The key used.*/
	const CryptoPP::SecByteBlock& m_key;
	/**The IV used.*/
	const CryptoPP::SecByteBlock& m_iv;
};

/**A filter to auto-decrypt data with AES.*/
class AESDecryptFilter : public cg::Filter
{
public:
	/**always returns false because the size will never change.*/
	virtual bool SizeChanges() const
	{
		return false;
	}
	/**Create it.
	\param key The key for decryption
	\param iv The IV for decryption*/
	AESDecryptFilter(const CryptoPP::SecByteBlock& key,
		const CryptoPP::SecByteBlock& iv)
		:m_key(key), m_iv(iv) {}
	/**Transform data in place (no copies).
	\param data The data place.
	\param size The data size.*/
	virtual void Transform(char* data, std::size_t size)
	{
		cg::SecureHelpers::AESDecrypt(data, data, size, size, m_key, m_iv);
	}
private:
	/**The key used.*/
	const CryptoPP::SecByteBlock& m_key;
	/**The IV used.*/
	const CryptoPP::SecByteBlock& m_iv;

};


}