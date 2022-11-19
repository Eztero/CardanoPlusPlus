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

void agregarUint64BytestoVector(std::vector <std::uint8_t> & bytesvector, std::uint64_t const & numero);

bool existen_coincidencias(std::uint8_t const * data1, std::uint8_t const * data2, std::uint16_t const data_len, std::uint16_t const ciclos ,std::uint16_t const salto );

bool existen_coincidencias_output(std::uint8_t const * data, std::uint8_t const * output, std::uint16_t const data_len, std::uint16_t const ciclos ,std::uint16_t const salto );

#endif
