/**
MIT License

Copyright (c) 2023 Eztero

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
https://github.com/input-output-hk/cardano-ledger/blob/master/eras/babbage/test-suite/cddl-files/babbage.cddl
**/

// 1 : [* transaction_output] --> transaction_output = { 0: address_keyhash, 1: amount, ? 2: datum_option, ? 3: script_ref}


#ifndef TRANSACTIONOUTPUT_HPP
#define TRANSACTIONOUTPUT_HPP
#include <cstdint>
#include <vector>
#include <memory>
#include "../Utils/cbor_lite.hpp"
#include "../Utils/txutils.hpp"
#include "../Hash/bech32.hpp"

class TransactionsOutputs {
public:
    explicit TransactionsOutputs();
    //TransactionsOutputs ~TransactionsOutputs();
    enum class ScriptType{
    Native_Script = 0,
    Plutus_Script_V1,
    Plutus_Script_V2
    };
    TransactionsOutputs &addOutput(std::uint8_t const *const address_keyhash, std::size_t address_keyhash_len, std::uint64_t const amount);
    TransactionsOutputs &addReturnColateral(std::uint8_t const *const address_keyhash, std::size_t address_keyhash_len, std::uint64_t const amount);
    //TransactionsOutputs &addOutput(std::string &payment_address, std::uint64_t const amount);
    TransactionsOutputs &addOutput(std::string payment_address, std::uint64_t const amount);
    TransactionsOutputs &addAsset(std::uint8_t const *const policyID, std::uint8_t const *const assetname, std::size_t const &assetname_len, std::uint64_t const amount);
    TransactionsOutputs &addAsset(std::uint8_t const *const policyID, std::string assetname, std::uint64_t const amount);
    TransactionsOutputs &addDatumHash(std::uint8_t const *const datum_hash, std::size_t &datum_hash_len);
    TransactionsOutputs &addReferenceScript(TransactionsOutputs::ScriptType script_type, std::uint8_t const *const script_, std::size_t &script_len );
    TransactionsOutputs &addDatumValue(std::uint64_t const datum_value);
    std::uint32_t const &getBodyMapcountbit() const;
    std::vector<std::uint8_t> const &getTransactionsOutputs();
    std::uint16_t const &getAmountTransactionsOutputs() const;
private:
    std::uint8_t outputmap_countbit;  // 0x01 = address , 0x02 = asset , 0x04 = datum , 0x08 = script , 0x10 = colateralreturn
    std::uint32_t pos_registro_elementos; // maximo 4294967295 , indica en que posicion del vector se deben registrar la cantidad de elementos
    std::uint32_t bodymap_countbit; ///  0x0002 , Tiene que iniciar con cero
    std::uint16_t tx_output_count;      // maximo 65535
    std::uint8_t addr_keyhash_buffer[BECH32_MAX_LENGTH];
    std::uint16_t addr_keyhash_buffer_len;
    std::size_t buff_sizet;
    std::vector<std::uint8_t> tx_output;
    std::vector<std::uint8_t> datum_hash;
    std::vector< std::vector<std::uint8_t> > capsula;
    std::vector<std::uint8_t> cbor_array;
    std::unique_ptr<CborSerialize> cbor;
    void getCborMultiassets();

};



#endif
