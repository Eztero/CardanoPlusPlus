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

#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP
#include <sodium.h>
#include "transaction_body.hpp"
#include "transaction_witness.hpp"
#include "auxiliary_data.hpp"
#include "certificates.hpp"
#include "metadata.hpp"
namespace Cardano{

class Transaction{
public:
    explicit Transaction();
    explicit Transaction(std::uint64_t txfeefixed, std::uint64_t txfeeperbytes);
    virtual ~Transaction();
    TransactionBody Body;
    TransactionWitness Witness;
    AuxiliaryData Auxiliarydata;
    Transaction &addExtendedSigningKey(std::uint8_t const *const xsk);
    std::uint64_t getFeeTransacion_PostBuild(std::uint64_t const number_of_signatures);

    std::vector<std::uint8_t> const &Build();
private:
    std::uint16_t witnessmapcountbit;
    std::uint64_t bytesskyesInwitness;
    std::uint8_t blake256[32];
    std::uint8_t body_signed[64];
    std::uint8_t xvkeys[96];
    unsigned int bytes_transaction;
    std::uint64_t feefixed;
    std::uint64_t feeperbytes;
std::vector<const std::uint8_t *> xskeys_ptr;
std::vector <std::uint8_t> cborTransaction;
};
}
#endif
