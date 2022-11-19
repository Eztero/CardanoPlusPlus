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
#include "../Utils/cbor_lite.hpp"
#include "../Utils/txutils.hpp"

class TransactionWitness {

public:
    explicit TransactionWitness();
    TransactionWitness &addVkeyWitness(std::uint8_t const *const public_key, std::uint8_t const * const signature_transactionbody );
    std::vector<std::uint8_t> const &Build();

private:
    std::uint8_t * ptrvec;
    std::size_t buff_sizet;
    std::uint8_t witnessmapcountbit;
    std::uint16_t vkeywitness_count; //maximo 65534
    std::vector <std::uint8_t> cbor_TransactionWitness;
    std::vector <std::uint8_t> vkeywitness;
    std::vector <std::uint8_t> native_script;

};

#endif
