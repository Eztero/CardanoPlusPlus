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
https://github.com/bitcoin/bips/blob/master/bip-0044.mediawiki
https://input-output-hk.github.io/adrestia/cardano-wallet/concepts/hierarchical-deterministic-wallets
**/

/**
--------------------   Bip44_ed25519 https://cips.cardano.org/cips/cip1852/  ------------------------

     m       /    1852'  /    1815'     /    0'..(2^31)'-1     /       0..2         /    0...(2^31)-1 (only 0 for role=2)
     :             :            :                :                      :                      :
(master key)   (purpose)   (coin type)       (account)                (role)               (index address)
               (constant)  (constant)
------------------------------------------------------------------------------------------------------

---------------------  Format Keys https://cips.cardano.org/cips/cip16/  -----------------------------

    xsk(extended signing key) = Extended Private Key (64 bytes) || Chain Code (32 bytes) = 96bytes
    xvk(extended verification key) = Public Key (32 bytes) || Chain Code (32 bytes) = 64bytes

    sk(signing key) = Private Key (32 bytes)
    vk(verification key) = Public Key (32 bytes)
------------------------------------------------------------------------------------------------------


----------------------  References of this library  --------------------------------------------------


 key_type -> 0=child private key  or  1=child public key

 address_type -> 0=payment normal, 1=payment enterprise ,stake =0;

 path ->    account_path   /       role_path   /      address_index_path
                :                      :                      :
            0=account 0         0=extern chain          0=address 0 (only 0 for staking key)
            1=account 1         1=intern chain          1=address 1
            ...                 2=staking key           .....
            nullptr=error       3=only account               nullptr=disable


 child_key_out  ->  keys xsk (if Key::Private) = Extended Private Key (64 bytes) || Chain Code (32 bytes) = 96bytes = XPUB_LENGTH (macro)
                                            or
                    keys xvk (if Key::Public) = Public Key (32 bytes) || Chain Code (32 bytes) = 64bytes = XPRV_LENGTH (macro)


**/

#ifndef BIP44_ED25519_HPP
#define BIP44_ED25519_HPP

#define H1815 2147485463U
#define H1852 2147485500U
#define H1854 2147485502U
#define H0 2147483648U

#include "../Bip32-ed25519/bip32_ed25519.hpp"
#include <string>

enum class Wallet{
HD,
MultiSignHD
};

enum class Role{
Extern,
Intern,
Staking,
Only_Account
};

enum class Network{
Testnet,
Mainnet
};

enum class Key{
Private,
Public
};

enum class Address{
Base,
Change_Base,
Enterprise,
Change_Enterprise,
Stake
};

///Keys XSK(Private) or XVK(Public) <-- /m/H1852/H1815/account_path/role_path/address_index_path
bool raw_derive_key(uint8_t const *const extended_master_secret_key, Wallet wallet_type, Key key_type,
                       uint32_t const *const account_path, Role role_path, uint32_t const *const address_index_path,
                       uint8_t *const child_key_out);


#endif // BIP44_ED25519_HPP
