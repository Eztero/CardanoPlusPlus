/**
MIT License

Copyright (c) 2017, 2021 Pieter Wuille
Modified by Eztero

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
https://github.com/bitcoin/bips/blob/master/bip-0173.mediawiki
https://github.com/sipa/bech32/tree/master/ref/c++
 **/


#ifndef BECH32_HPP_INCLUDED
#define BECH32_HPP_INCLUDED
#include <sodium.h>
#include <new>
#include <string>
#include <cstring>
#include <cstdlib>
#include "../Utils/cmacros.hpp"

#define SEPARATOR_BECH32 '1' //Bech32

///'hrp'(human readable part) es el prefijo ejemplo addr_,acct_
///'data' son los datos que se codificaran o decodificaran en bech32, como por ejemplo una llave publica en formato hex
namespace Cardano{
namespace Hash{


///  bech32_encode { (string)hrp | (uint8_t)data[data_len] } = (string)encode_out
bool const bech32_encode(char const *const hrp, std::uint8_t const *const data, std::uint16_t const data_len, std::string &encode_out) noexcept;

///  bech32_decode { (char)bech32_code } = (uint8_t)data[data_len] ; data_len puede ser nullprt, data_len <= 57 bytes
bool const bech32_decode(char const *const bech32_code,std::uint8_t *const data_out,std::uint16_t *const data_out_len) noexcept;
}
}

#endif // BECH32_HPP_INCLUDED
