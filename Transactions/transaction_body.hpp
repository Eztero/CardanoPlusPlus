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

#include <sodium.h>
#include <vector>
#include <string>
#include <cstdint>
#include "certificates.hpp"
#include "multiassets.hpp"
#include "../Utils/cbor_lite.hpp"
#include "../Utils/txutils.hpp"
#include "../Hash/bech32.hpp"
#include "transactionoutput.hpp"
#include "transactioninput.hpp"

class TransactionBody : private Certificates, Multiassets {

public:

    explicit TransactionBody();
    virtual ~TransactionBody();
    TransactionsOutputs TransactionOutput;
    TransactionsInputs TransactionInput;
    TransactionBody & addFee(std::uint64_t const amount);
    TransactionBody & addInvalidAfter(std::uint64_t const number);
    TransactionBody & addInvalidBefore(std::uint64_t const number);
    TransactionBody & addAuxiliaryDataHash(std::uint8_t const *const hash_32bytes);
    TransactionBody & addWithdrawals(std::uint8_t const *const stake_address_keyhash, std::uint64_t const amount);
    TransactionBody & addWithdrawals(std::string &stake_address, std::uint64_t const amount);
    TransactionBody & addTotalCollateral(std::uint64_t const amount);
    std::vector<std::uint8_t> const & Build();
    std::vector<std::uint8_t> const & getcborDatums_afterBuild() const;
    std::vector<std::uint8_t> const & getcborRedeemers_afterBuild() const;
    std::uint16_t const & getWitnessMapcountbit() const;

private:

    std::uint8_t const *ptrvec;
    std::size_t buff_sizet;
    std::uint32_t buff_uint32t;
    std::uint8_t addr_keyhash_buffer[BECH32_MAX_LENGTH]{};
    std::uint16_t addr_keyhash_buffer_len;
    std::uint32_t bodymapcountbit; // pone un bits a 1 si existe la variable, en la posisicion correspondiente al map de el transaccion body
    std::uint16_t witnessmapcountbit; // pone un bits a 1 si existe la variable, en la posisicion correspondiente al map de el transaccion witness
    std::uint16_t withdrawals_count; //maximo 65534
    std::uint64_t fee;
    std::uint64_t ttl;  // time to alive
    std::uint64_t vis;  // validity interval start
    std::uint64_t totalcollateral;  // validity interval start
    CborSerialize cbor;
    std::vector <std::uint8_t>cbor_redeemers{};
    std::vector <std::uint8_t>cbor_datums{};
    std::vector <std::uint8_t>withdrawals{};
    std::vector <std::uint8_t>update{};
    std::vector <std::uint8_t>auxiliary_data_hash{};
    std::vector <std::uint8_t>validity_interval_start{};
    std::vector <std::uint8_t>mint{};
    std::vector <std::uint8_t>collateral_inputs{};
    std::vector <std::uint8_t>required_signers{};
    std::vector <std::uint8_t>network_id{};
    std::vector <std::uint8_t>collateral_return{};
    std::vector <std::uint8_t>total_collatera{};
    std::vector <std::uint8_t>reference_inputs{};
    std::uint8_t V1language_views[444];
    std::uint8_t V2language_views[467];
};


#endif
