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

#ifndef WITHDRAWAL_HPP
#define WITHDRAWAL_HPP

#include <vector>
#include <cstdint>
#include <string>
#include <cstring>
#include "../Utils/cbor_lite.hpp"
#include "../Utils/txutils.hpp"
#include "../Utils/plutusjsonschema.hpp"
#include "../Hash/bech32.hpp"

namespace Cardano{
    
class Withdrawals{
public:
explicit Withdrawals();
virtual ~Withdrawals();
Withdrawals & addWithdrawals(std::uint8_t const * const raw_stake_address, std::uint64_t const amount);
Withdrawals & addWithdrawals(std::string & stake_address, std::uint64_t const amount);
void addWithdrawalRedeemer( std::string & json_redeemer, std::uint64_t const cpusteps, std::uint64_t const memoryunits );
void alphanumeric_organization();
std::uint16_t const & getWithdrawalRedeemersCount() const;
std::uint32_t const & getBodyMapcountbit() const;
std::uint16_t const & getWitnessMapcountbit() const;
std::uint16_t const & getWithdrawalsCount() const;
std::vector<std::uint8_t> const & getWithdrawals() const;
std::vector<std::uint8_t> const & getWithdrawalRedeemers() const;

private:
std::size_t buff_sizet;
std::uint32_t bodymapcountbit;
std::uint16_t  witnessmap_countbit;
std::uint16_t withdrawals_count; //maximo 65534
std::uint16_t redeemer_withdrawals_count;
std::uint8_t buffbech32[BECH32_MAX_LENGTH]{};
std::vector <std::uint8_t>withdrawals{};
std::vector <std::uint8_t> redeemer_withdrawals{};

};
}


#endif
