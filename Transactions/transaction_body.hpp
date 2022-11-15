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


#ifndef TRANSACTION_BODY_HPP
#define TRANSACTION_BODY_HPP

#include <vector>
#include <string>
#include <cstdint>
#include "certificates.hpp"
#include "../Utils/cbor_lite.hpp"
#include "../Hash/bech32.hpp"

class TransaccionBody : private CborSerialize{

public:

    explicit TransaccionBody(std::vector <std::uint8_t> * const transactionbody_cbor_out);
    virtual ~TransaccionBody();
    TransaccionBody &addTransactionsInput(std::uint8_t const *const TxHash, std::uint64_t const TxIx);
    TransaccionBody &addTransactionsOutput(std::uint8_t const *const address_keyhash, std::size_t address_keyhash_len, std::uint64_t const amount);
    TransaccionBody &addTransactionsOutput(std::string &payment_address, std::uint64_t const amount);
    TransaccionBody &addFee(std::uint64_t const amount);
    TransaccionBody &addInvalidAfter(std::uint64_t const number);
    TransaccionBody &addInvalidBefore(std::uint64_t const number);
    TransaccionBody &addAuxiliaryDataHash(std::uint8_t const *const hash_32bytes);
    TransaccionBody &addCertificates(Certificates *cert);
    TransaccionBody &addWithdrawals(std::uint8_t const *const stake_address_keyhash, std::uint64_t const amount);
    TransaccionBody &addWithdrawals(std::string &stake_address, std::uint64_t const amount);
    void Build();

private:

    Certificates *cert_;
    std::uint8_t *ptr_data;
    std::size_t buff_sizet;
    std::uint8_t addr_keyhash_buffer[BECH32_MAX_LENGTH];
    std::uint16_t addr_keyhash_buffer_len;
    std::uint16_t bodymapcountbit; // pone un bits a 1 si existe la variable, en la posisicion correspondiente al map de el transaccion body
    std::uint16_t output_count;
    std::uint16_t input_count;
    std::uint16_t withdrawals_count;
    std::uint64_t fee;
    std::uint64_t ttl;  // time to alive
    std::uint64_t vis;  // validity interval start
    std::vector <std::uint8_t>input;
    std::vector <std::uint8_t>output;
    std::vector <std::uint8_t>output_datum_hash;
    std::vector <std::uint8_t>certificate;
    std::vector <std::uint8_t>withdrawals;
    std::vector <std::uint8_t>update;
    std::vector <std::uint8_t>auxiliary_data_hash;
    std::vector <std::uint8_t>validity_interval_start;
    std::vector <std::uint8_t>mint;
    std::vector <std::uint8_t>script_data_hash;
    std::vector <std::uint8_t>collateral_inputs;
    std::vector <std::uint8_t>required_signers;
    std::vector <std::uint8_t>network_id;
    std::vector <std::uint8_t>collateral_return;
    std::vector <std::uint8_t>total_collatera;
    std::vector <std::uint8_t>reference_inputs;
    void addUint64BytestoVector(std::vector <std::uint8_t> & bytesvector, std::uint64_t const & numero);
    bool existen_coincidencias(std::uint8_t const * data1, std::uint8_t const * data2, std::uint16_t const data_len, std::uint16_t const ciclos ,std::uint16_t const salto );
    bool existen_coincidencias_output(std::uint8_t const * data, std::uint8_t const * output, std::uint16_t const data_len, std::uint16_t const ciclos ,std::uint16_t const salto );
};


#endif
