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

#ifndef MULTIASSETS_HPP
#define MULTIASSETS_HPP

#include <cstdint>
#include <vector>
#include <string>
#include <new>

#include "../Utils/cbor_lite.hpp"

class Multiassets{
public:
    explicit Multiassets();
    virtual ~Multiassets();
    Multiassets &addAsset(std::uint8_t const *const policyID, std::uint8_t const *const assetname, std::size_t const &assetname_len, std::uint64_t const amount);
    Multiassets &addAsset(std::uint8_t const *const policyID, std::string assetname, std::uint64_t const amount);
    //std::uint8_t const &minUTXORequired();
    std::vector<std::uint8_t> const &getCborMultiassets();
private:

    //std::vector<std::uint8_t> buffer_cbor;
    std::vector< std::vector<std::uint8_t> > capsula;
    //CborSerialize *cbor; //se usa buffer_cbor para serializar en cbor
    CborSerialize cbor;
};

#endif
