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

#include "NetworkObject.hpp"
#include "ArrayView.hpp"

namespace cg {
namespace net {
/**An interface for encrypting data.*/
class IEncryptor : public cg::net::NetworkObject
{
public:
	/**Encrypt data
	\param src A cg::ArrayView with the data to encrypt.
	\param dest The destination. If it is nullptr, then the src will be over
	written with the encrypted data.  If not nullptr dest should not be
	allocated, but should be default initialized.*/
	virtual void Encrypt(cg::ArrayView& src, cg::ArrayView* dest) = 0;
	/**Encrypt data
	\param src A cg::ArrayView with the data to decrypt.
	\param dest The destination. If it is nullptr, then the src will be over
	written with the decrypted data.  If not nullptr dest should not be
	allocated, but should be default initialized.*/
	virtual void Decrypt(cg::ArrayView& src, cg::ArrayView* dest) = 0;
};

}
}