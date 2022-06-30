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
https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki
**/


#ifndef BIP39_HPP
#define BIP39_HPP
#include <sodium.h> ///library libsodium ,init with sodium_init()
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include "diccionary.hpp"

#include <iostream>

///Returns its entropy and its length in bytes ; if there is an error it returns a nullptr; free memory with free()
std::uint8_t *mnemotic2entropy(char const *const mnemotic,char const diccionary[][2048],std::uint8_t *const entropy_length_bytes);

///Returns its mnemotics and its length in bytes, valid entropy range 128 - 256 bits ; if there is an error it returns a nullptr; free memory with free()
char *entropy2mnemotic(std::uint8_t const *const entropy, std::uint8_t const *const entropy_length_bytes, char const diccionary[][2048], std::uint8_t *const mnemotic_length);


#endif
