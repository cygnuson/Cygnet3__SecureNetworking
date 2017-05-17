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