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

#ifndef TXUTILS_HPP
#define TXUTILS_HPP

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include "cenum.hpp"

namespace Cardano{
    namespace Utils{

unsigned int const bytes_structure_cbornumber(std::uint64_t number) noexcept;

void addUint64toVector(std::vector <std::uint8_t> & bytesvector, std::uint64_t const & numero);
void addUint16toVector(std::vector <std::uint8_t> & bytesvector, std::uint16_t const & numero);
void addUint16toVector(std::vector <std::uint8_t> *& bytesvector, std::uint16_t *& numero);
void replaceUint16toVector(std::uint8_t * bytesvector, std::uint16_t const & numero) noexcept;

std::uint64_t const extract8bytestoUint64(std::uint8_t  const * const array8bytes) noexcept;
std::uint16_t const extract2bytestoUint16(std::uint8_t  const * const array2bytes) noexcept;

bool const existen_coincidencias(std::uint8_t const * data1, std::uint8_t const * data2, std::uint16_t const data_len, std::uint16_t const ciclos, std::uint16_t const salto ) noexcept;

bool const existen_coincidencias_output(std::uint8_t const * data, std::uint8_t const * output, std::uint16_t const data_len, std::uint16_t const ciclos, std::uint16_t const salto ) noexcept;

std::uint8_t const * const hexchararray2uint8array(std::string const & string_hex, std::size_t * hexchararray2uint8array_len) noexcept; // free memory with delete[]

    }
}
#endif
