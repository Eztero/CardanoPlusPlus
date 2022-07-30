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
     :                :                      :                :                     :                      :
(master key)     (purpose/Wallet)        (coin type)       (account)              (Role)              (index address)
Array(uint8_t)    0=HD (1852)            (constant)        Number(uint32)         0=Extern             Number(uint32)
                  1=MultiSignHD (1854)                                            1=Intern
                                                                                  2=Staking
                                                                                  3=OnlyAccount (only generate account address)
-------------------------------------  Library functions  ---------------------------------------------------

    derive_master_key(master key, Wallet::HD,  Key::Private,  account, Role::Extern, index_addr, bech32_raw_child_key_out)
raw_derive_master_key(master key, Wallet::HD,  Key::Private,  account, Role::Extern, index_addr, raw_child_key_out)
                                             :
                                        0=Private(xsk)
                                        1=Public (xvk)

    derive_account_key(raw account key, AccountKeyType::Account_xvk, Wallet::HD, Key::Private, Role::Extern, index_addr, bech32_raw_child_key_out);
raw_derive_account_key(raw account key, AccountKeyType::Account_xvk, Wallet::HD, Key::Private, Role::Extern, index_addr, raw_child_key_out);
                                            :
                                        0=Account_xvk
                                        1=Account_xsk


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

#include "../Bip32-ed25519/bip32_ed25519.hpp"
#include "../Hash/bech32.hpp"
#include <string>

enum class Wallet{
HD,
MultiSignHD
};

enum class Role{
Extern,
Intern,
Staking,
OnlyAccount
};

enum class Key{
Private,
Public
};

enum class AccountKeyType{
Account_xvk,
Account_xsk
};

///raw_derive_key(master key) --> / master key / H1852 / H1815 / account_path / role_path / index_addr Keys xsk(Private) or xvk(Public)
///                           --> / master key / H1852 / H1815 / account_path Keys xsk(Private) or xvk(Public)
bool raw_derive_master_key(uint8_t const *const extended_master_secret_key, Wallet wallet_type, Key key_type,
                           uint32_t const account_path, Role role_path, uint32_t const address_index_path,
                           uint8_t *const raw_child_key_out);

///derive_key(master key) --> / master key / H1852 / H1815 / account_path / role_path / index_addr Keys xsk(Private) or xvk(Public)
///                       --> / master key / H1852 / H1815 / account_path Keys xsk(Private) or xvk(Public)
bool derive_master_key(std::uint8_t const *const extended_master_secret_key, Wallet wallet_type, Key key_type,
                       std::uint32_t const account_path, Role role_path, std::uint32_t const address_index_path,
                       std::string& bech32_raw_child_key_out);

///raw_derive_account_key(raw account xvk/xsk)  --> / .. / .. / .. / account / role / index_addr Keys xsk(Private) or xvk(Public)
bool raw_derive_account_key(std::uint8_t const *const account_key, AccountKeyType account_key_type ,Wallet wallet_type,
                            Key key_type, Role role_path, std::uint32_t const address_index_path,
                            std::uint8_t *const raw_child_key_out);

///derive_account_key(raw account xvk/xsk)  --> / .. / .. / .. / account / role / index_addr Keys xsk(Private) or xvk(Public)
bool derive_account_key(std::uint8_t const *const account_key, AccountKeyType account_key_type, Wallet wallet_type,
                        Key key_type, Role role_path, std::uint32_t const address_index_path,
                        std::string& bech32_raw_child_key_out);

#endif // BIP44_ED25519_HPP
