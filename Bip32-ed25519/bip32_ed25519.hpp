/**
MIT License

Copyright (c) 2022 Eztero

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Documentation:
https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki
https://input-output-hk.github.io/adrestia/cardano-wallet/concepts
https://cips.cardano.org/cips/cip16/
https://cips.cardano.org/cips/cip1852/
https://libsodium.gitbook.io/doc/advanced/point-arithmetic
**/


#ifndef BIP32_ED25519_HPP
#define BIP32_ED25519_HPP

#include "../Utils/cmacros.hpp"
#include "../Hash/pbkdf2_hmac512_libsodium.hpp"
#include <cstdint>
#include <cstring>
#include <sodium.h>


///las llaves se codificaran ahora en el formato xsk y xvk propuesto en https://cips.cardano.org/cips/cip16/

namespace Cardano{

bool getRawMasterKey( std::uint8_t const * const entropy, std::size_t const entropy_len, std::uint8_t const * const password, std::size_t const password_len, std::uint8_t * const mastersecretkey_out ) noexcept;

bool rawprivatekey_to_rawpublickey( std::uint8_t const * const raw_privatekey_xsk, std::uint8_t * const raw_publickey_xvk ) noexcept;

bool raw_child_privatekey( std::uint8_t const * const raw_parent_privatekey_xsk, std::uint32_t const index, std::uint8_t * const raw_child_privatekey_xsk ) noexcept;

bool raw_child_publickey( std::uint8_t const * const raw_parent_public_key_xvk, std::uint32_t const index, std::uint8_t * const raw_child_public_key_xvk ) noexcept;

bool signature( std::uint8_t const * const raw_privatekey_xsk, std::uint8_t const * const message, std::size_t const message_len, std::uint8_t * const out ) noexcept;

bool verify( std::uint8_t const * const raw_publickey, std::uint8_t const * const message, const std::uint8_t message_len, std::uint8_t const * const signature ) noexcept;

bool valid_ed25519_sk( std::uint8_t const * const raw_privatekey_sk ) noexcept;

}

#endif // BIP32_ED25519_HPP
