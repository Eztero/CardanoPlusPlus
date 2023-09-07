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
https://github.com/input-output-hk/cardano-ledger/blob/master/eras/babbage/test-suite/cddl-files/babbage.cddl
**/

#ifndef TRANSACTION_WITNESS_HPP
#define TRANSACTION_WITNESS_HPP

#include <vector>
#include <cstdint>
#include "../utils/cbor_lite.hpp"


namespace Cardano{

class TransactionWitness {

public:
    explicit TransactionWitness();
    virtual ~TransactionWitness();
    TransactionWitness & addVkeyWitness( std::uint8_t const * const public_key, std::uint8_t const * const signature_transactionbody );
    //TransactionWitness & addNativeScript( std::uint8_t const * const cborNativeScript, std::size_t const cborNativeScript_len );
    TransactionWitness & addNativeScript( std::vector<std::uint8_t> const & cborNativeScript );
    TransactionWitness & addRedeemer( std::vector <std::uint8_t> const & cborRedeemers);
    TransactionWitness & addDatum(std::vector <std::uint8_t> const & cborDatums);
    TransactionWitness & addPlutusV1Script(std::vector <std::uint8_t> const & cborPlutusV1Scripts);
    TransactionWitness & addPlutusV2Script(std::vector <std::uint8_t> const & cborPlutusV2Scripts);
    std::vector<std::uint8_t> const & Build();

private:
    std::uint8_t * ptrvec;
    std::size_t buff_sizet;
    std::uint8_t witnessmapcountbit;
    std::uint16_t vkeywitness_count; //maximo 65534
    std::uint16_t cbor_plutusv1scripts_count{0};
    std::uint16_t cbor_plutusv2scripts_count{0};
    std::uint16_t cbor_native_script_count{0};
    Utils::CborSerialize cbor;
    std::vector <std::uint8_t> vkeywitness{};
    std::vector <std::uint8_t> cbor_datums{};
    std::vector <std::uint8_t> cbor_redeemers{};
    std::vector <std::uint8_t> const *cbor_plutusv1scripts{};
    std::vector <std::uint8_t> const *cbor_plutusv2scripts{};
    std::vector <std::uint8_t> const *cbor_native_script{};

};
}
#endif
