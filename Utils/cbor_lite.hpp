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
https://www.rfc-editor.org/rfc/rfc8949
**/


#ifndef CBOR_LITE_HPP
#define CBOR_LITE_HPP
#include <cstdint>
#include <string>
#include <vector>

class CborSerialize
{
public:
    explicit CborSerialize();
    virtual ~CborSerialize();

    CborSerialize &createArray(std::uint64_t const size_array);
    CborSerialize &createArrayUndefined();  // stop whith addBreak()
    CborSerialize &createMap(std::uint64_t const size_array);
    CborSerialize &addIndexMap(std::uint64_t const index);
    CborSerialize &addIndexMap(std::string const &text);
    CborSerialize &addIndexMap(std::uint8_t const *const bytesarray, std::uint64_t bytesarray_length);
    CborSerialize &addIndexMap(std::uint8_t const * const arraynumbe8byteshex);
    CborSerialize &addBool(bool const b);
    CborSerialize &addNull();
    CborSerialize &addBreak();
    CborSerialize &addUint(std::uint64_t const number);
    CborSerialize &addUint(std::uint8_t const * const arraynumbe8byteshex);
    CborSerialize &addNint_withoutzero(std::uint64_t number); // toma un uint64_t y lo serializa como un numero negativo de 64bytes, se excluye el cero
    CborSerialize &addNint_zero_equal_1(std::uint64_t number); //considera cero = -1
    CborSerialize &addTag(std::uint64_t const number); //funcion addTag incompleta,los primeros 23 numeros estan reservados a funciones especiales
    CborSerialize &addBytesArray(std::uint8_t const *const bytes, std::uint64_t const bytes_length);
    CborSerialize &addBytesArray(std::vector<std::uint8_t> const &bytes);
    CborSerialize &addBytesArray();
    CborSerialize &addUint2BytesArray(std::uint64_t const number);
    CborSerialize &addString(std::string const &text);
    CborSerialize &bypassVectorCbor(std::vector<std::uint8_t> const &vectorCbor);
    CborSerialize &bypassIteratorVectorCbor(std::vector<std::uint8_t>::const_iterator it_begin, std::vector<std::uint8_t>::const_iterator it_end);
    CborSerialize &bypassPtrUint8Cbor( std::uint8_t const * const ptrArrayCbor, std::uint64_t const ptrArrayCbor_len );
    void clearCbor();
    std::vector<std::uint8_t> const & getCbor() const;

private:
    enum class Pos_hex{
        hff,
        hff2,
        hff4,
        hff8,
    };
    std::vector<std::uint8_t> bytes_cbor_data{};
    void AddNumber2Vector(std::uint64_t const &size_array, Pos_hex const &pos);
    void AddNumber2Vector(std::uint64_t const &size_array, Pos_hex const &pos, std::vector<std::uint8_t> &Vector_);
};

#endif // CBOR_LITE_HPP
