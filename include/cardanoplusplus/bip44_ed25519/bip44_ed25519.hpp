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
https://cips.cardano.org/cips/cip5/
https://cips.cardano.org/cips/cip11/
https://cips.cardano.org/cips/cip21/
https://cips.cardano.org/cips/cip1852/
https://cips.cardano.org/cips/cip1854/
https://github.com/bitcoin/bips/blob/master/bip-0044.mediawiki
https://input-output-hk.github.io/adrestia/cardano-wallet/concepts/hierarchical-deterministic-wallets
**/

/**
--------------------   Bip44_ed25519 https://cips.cardano.org/cips/cip1852/  ------------------------

     m        /    1852' or 1854'     /    1815'     /    0'..(2^31)'-1     /      0..2       /    0...(2^31)-1 (only 0 for Role=2)
     :                   :                   :                :                     :                      :
(master key)     (purpose/Wallet)        (coin type)       (account)              (Role)              (index address)
Array(uint8_t)    0=HD (1852)            (constant)        Number(uint32)         0=Extern             Number(uint32)
                  1=MultiSignHD (1854)                                            1=Intern
                                                                                  2=Staking
                                                                                  3=OnlyAccount (only generate account address)

-------------------------------------  Library functions  ---------------------------------------------------
                                                                                                                             Key with prefix in bech32 format (string)
                                                                                                                                      :
    derivekey(input_key, InputKey::ExtendedMasterKey, Wallet::HD, OutputKey::Private,   account,   Role::Extern, index address, bech32_output_key)
raw_derivekey(input_key, InputKey::ExtendedMasterKey, Wallet::HD, OutputKey::Private,   account,   Role::Extern, index address, output_key);
                 :            :                                       :                    :                                          :
           Master Key     0=ExtendedMasterKey                      0=Private(xsk)      disabled if                            Format in xsk or xvk (hex)
           Account Key    1=AccountKey_xvk                         1=Public (xvk)      InputKey = AccountKey_xxx
                          2=AccountKey_xsk


---------------------  Format Keys https://cips.cardano.org/cips/cip16/  -----------------------------

  xsk(extended signing key)      =  Extended Private Key (64 bytes) || Chain Code (32 bytes) = 96 bytes (XSK_LENGTH)
  xvk(extended verification key) =  Public Key (32 bytes) || Chain Code (32 bytes)           = 64 bytes (XVK_LENGTH)

  sk(signing key)      =   Private Key  = 32 bytes
  vk(verification key) =   Public Key   = 32 bytes
------------------------------------------------------------------------------------------------------
**/

#ifndef BIP44_ED25519_HPP
#define BIP44_ED25519_HPP

#define H1815 2147485463U
#define H1852 2147485500U
#define H1854 2147485502U
#define H0 2147483648U

#include "../utils/cenum.hpp"
#include <string>
#include <cstring>

#include <cstdint>

namespace Cardano{

//for generate all key
bool getRawKey( Cardano::InputKey input_key_type, std::uint8_t const * const input_key, Cardano::Wallet wallet_type, Cardano::OutputKey output_key_type,
                std::uint32_t const account_path, Cardano::Role role_path, std::uint32_t const address_index_path,
                std::uint8_t * const output_key) noexcept;

//for generate only key account
//mandatory, role_path = Cardano::Role::OnlyAccount
bool getRawKey( Cardano::InputKey input_key_type, std::uint8_t const * const input_key, Cardano::Wallet wallet_type, Cardano::OutputKey output_key_type,
                std::uint32_t const account_path, Cardano::Role role_path, std::uint8_t * const output_key) noexcept;

//for generate all key
bool getBech32key( Cardano::InputKey input_key_type, std::uint8_t const * const input_key, Cardano::Wallet wallet_type, Cardano::OutputKey output_key_type,
                   std::uint32_t const account_path, Cardano::Role role_path, std::uint32_t const address_index_path,
                   std::string & bech32_output_key) noexcept;

//for generate only key account
//mandatory, role_path = Cardano::Role::OnlyAccount
bool getBech32key( Cardano::InputKey input_key_type, std::uint8_t const * const input_key, Cardano::Wallet wallet_type, Cardano::OutputKey output_key_type,
                   std::uint32_t const account_path, Cardano::Role role_path, std::string & bech32_output_key) noexcept;

}

#endif // BIP44_ED25519_HPP
