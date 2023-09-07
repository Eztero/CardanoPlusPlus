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
https://cips.cardano.org/cips/cip19/
https://hydra.iohk.io/build/16861845/download/1/ledger-spec.pdf (p. 111)
**/

/**

-------------------------------------  Library functions  ---------------------------------------------------

                                                                                                         Number(uint32)   Number(uint32)    String (addr1.., stake1..)
                                                                                                                :                :               :
    getBech32Address(input_key, InputKey::ExtendedMasterKey, Network::Mainnet, Wallet::HD, Address::Base, account_path, address_index_path, address_out);
       getRawAddress(input_key, InputKey::ExtendedMasterKey, Network::Mainnet, Wallet::HD, Address::Base, account_path, address_index_path, output_keyhash, output_keyhash_len)
                             :           :                            :               :             :                                               :              :
                          Master Key    0=ExtendedMasterKey       0=Testnet         0=HD         0=Base                                         uint8_t[output_keyhash_len]
                          Account Key   1=AccountKey_xvk          1=Mainnet         1=MultiSign  1=Base_Change
                                        2=AccountKey_xsk                                         2=Enterprise
                                                                                                 3=Enterprise_Change
                                                                                                 4=Stake

**/

#ifndef ADDRESSES_HPP
#define ADDRESSES_HPP

#include <stdexcept>
#include <sodium.h>
#include "../utils/cenum.hpp"

namespace Cardano{

// Both functions throw exceptions of type std::invalid_argument

// get a addresses serialized in bech32, for example addr1v8....
void getBech32Address(std::uint8_t const * const input_key, Cardano::InputKey const input_key_type, Cardano::Network const network_id, Cardano::Wallet const wallet_type, Cardano::Address const address_type,
                      std::uint32_t const account_path, std::uint32_t const address_index_path, std::string & address_out);

// get unserialized addresses
void getRawAddress(std::uint8_t const * const input_key, Cardano::InputKey const input_key_type, Cardano::Network const network_id, Cardano::Wallet const wallet_type, Cardano::Address const address_type,
                               std::uint32_t const account_path, std::uint32_t const address_index_path,
                               std::uint8_t * const output_raw, std::uint8_t * const output_raw_len);

void getBech32ScriptHash(std::uint8_t const * const input_script, std::size_t input_script_len, Cardano::ScriptType const script_type, std::string & address_out );


void getRawScriptHash(std::uint8_t const * const input_script, std::size_t input_script_len, Cardano::ScriptType const script_type,
                      std::uint8_t * const output_28bytesraw, std::uint8_t * const output_28bytesraw_len );

void getBech32AddressfromScript(std::string const &script, Cardano::ScriptType const script_type,  Cardano::Network const network_id,
                                Cardano::ScriptAddress const address_type, std::string & address_out );

}

#endif // ADDRESSES_HPP
