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

**/

#ifndef METADATA_HPP
#define METADATA_HPP

#include <vector>
#include <cstdint>
#include "../Utils/cbor_lite.hpp"
#include "../Utils/txutils.hpp"

class Metadatas{
public:
    explicit Metadatas();
void addMetadata(std::uint64_t const keytag, std::vector<std::uint8_t> const &CborMetadata);
//Metadatas &addMetadata(std::uint64_t const keytag, std::string const metadata);  ///SIN TERMINAR
bool arethereMetadatas() const;
std::vector<std::uint8_t> const &getCborMetadatas();
private:
std::uint8_t * ptrvec;
std::uint16_t metadata_count;
std::vector <std::uint8_t> metadata;
CborSerialize cbor;
//std::vector <std::uint8_t> metadata_cbor;
};

#endif
